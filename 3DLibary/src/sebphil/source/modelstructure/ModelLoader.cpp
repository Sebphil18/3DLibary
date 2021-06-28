#include "modelstructure/ModelLoader.h"
#include <iostream>
#include "modelstructure/ModelDataConverter.h"
#include "exceptions/ModelLoadException.h"

namespace glib {

	ModelLoader::ModelLoader(const std::string& filePath) {

		this->filePath = filePath;
		resolveFilepath();

		worker = std::async(std::launch::async, &ModelLoader::loadModel, this);
	}

	void ModelLoader::resolveFilepath() {

		std::size_t pathEnd = filePath.find_last_of("/");
		directory = filePath.substr(0, pathEnd);
		fileName = filePath.substr(pathEnd + 1, filePath.length());
	}

	void ModelLoader::loadModel() {

		// block if data is already loading; the new data will be loaded when the current loading process is finished
		std::scoped_lock<std::mutex> lock(loadingLock);

		Assimp::Importer importer;
		scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality);

		tryLoadModel(importer);
	}

	void ModelLoader::tryLoadModel(const Assimp::Importer& importer) {

		try {

			if (!scene)
				throw ModelLoadException(*this);

			examinNode(scene->mRootNode);

		} catch (const ModelLoadException& exception) {

			std::cout << exception.what() << "\n";
			std::cout << importer.GetErrorString() << "\n\n";
		}
	}

	void ModelLoader::examinNode(aiNode* node) {

		loadMeshes(node);
		expandNode(node);
	}

	void ModelLoader::loadMeshes(aiNode* node) {

		for (std::size_t i = 0; i < node->mNumMeshes; i++) {

			std::uint32_t meshIndex = node->mMeshes[i];
			aiMesh* mesh = scene->mMeshes[meshIndex];
			
			DeferredMeshData meshData;
			meshData.vertices = loadVertices(mesh);
			meshData.indices = loadIndices(mesh);
			// TODO: input variable
			loadMaterial(mesh, meshData.material);

			data.meshes.push_back(meshData);
		}
	}

	std::vector<Vertex> ModelLoader::loadVertices(aiMesh* mesh) {

		std::vector<Vertex> vertices;

		for (std::uint32_t vertIndex = 0; vertIndex < mesh->mNumVertices; vertIndex++) {

			Vertex vertex = getVertex(mesh, vertIndex);
			setTangents(vertIndex, mesh, vertex);

			vertices.push_back(vertex);
		}

		return vertices;
	}

	Vertex ModelLoader::getVertex(aiMesh* mesh, std::uint32_t vertexIndex) {

		Vertex vertex = { glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0) };
		vertex.position = getPosition(vertexIndex, mesh);
		vertex.normal = getNormal(vertexIndex, mesh);
		vertex.texCoord = getTexCoords(vertexIndex, mesh);

		return vertex;
	}

	glm::vec3 ModelLoader::getPosition(std::uint32_t vertexIndex, aiMesh* mesh) {
		
		aiVector3D position = mesh->mVertices[vertexIndex];
		
		return convertVector(position);
	}

	glm::vec3 ModelLoader::getNormal(std::uint32_t vertexIndex, aiMesh* mesh) {

		aiVector3D normal = mesh->mNormals[vertexIndex];

		return convertVector(normal);
	}

	void ModelLoader::setTangents(std::uint32_t vertexIndex, aiMesh* mesh, Vertex& vertex) {

		if (mesh->HasTangentsAndBitangents()) {
			vertex.tangent = getTangent(vertexIndex, mesh);
			vertex.bitangent = getBitangent(vertexIndex, mesh);
		} else {
			vertex.tangent = glm::vec3(0);
			vertex.bitangent = glm::vec3(0);
		}
	}

	glm::vec3 ModelLoader::getTangent(std::uint32_t vertexIndex, aiMesh* mesh) {

		aiVector3D tangent = mesh->mTangents[vertexIndex];

		return convertVector(tangent);
	}

	glm::vec3 ModelLoader::getBitangent(std::uint32_t vertexIndex, aiMesh* mesh) {

		aiVector3D bitangent = mesh->mBitangents[vertexIndex];

		return convertVector(bitangent);
	}

	glm::vec2 ModelLoader::getTexCoords(std::uint32_t vertexIndex, aiMesh* mesh) {

		glm::vec2 texCoord(0);
		if (mesh->mTextureCoords[0]) {
			texCoord.x = mesh->mTextureCoords[0][vertexIndex].x;
			texCoord.y = mesh->mTextureCoords[0][vertexIndex].y;
		}

		return texCoord;
	}

	glm::vec3 ModelLoader::convertVector(aiVector3D aiVec3) {
		return glm::vec3(aiVec3.x, aiVec3.y, aiVec3.z);
	}

	glm::vec2 ModelLoader::convertVector(aiVector2D aiVec2) {
		return glm::vec2(aiVec2.x, aiVec2.y);
	}

	std::vector<std::uint32_t> ModelLoader::loadIndices(aiMesh* mesh) {

		std::vector<std::uint32_t> indices;

		for (std::size_t j = 0; j < mesh->mNumFaces; j++) {

			aiFace face = mesh->mFaces[j];

			for (std::size_t index = 0; index < face.mNumIndices; index++)
				indices.push_back(face.mIndices[index]);
		}

		return indices;
	}

	// TODO: textureTypes
	void ModelLoader::loadMaterial(aiMesh* mesh, DeferredMaterial& material) {

		std::uint32_t matIndex = mesh->mMaterialIndex;
		if (matIndex >= 0) {

			aiMaterial* aiMaterial = scene->mMaterials[matIndex];

			loadTextures(aiTextureType_DIFFUSE, aiMaterial, material);
			loadTextures(aiTextureType_METALNESS, aiMaterial, material);
			loadTextures(aiTextureType_NORMALS, aiMaterial, material);
			loadTextures(aiTextureType_HEIGHT, aiMaterial, material);
			loadTextures(aiTextureType_DIFFUSE_ROUGHNESS, aiMaterial, material);

			loadMaterialProperties(aiMaterial, material);
		}
	}

	void ModelLoader::loadTextures(aiTextureType type, aiMaterial* aiMaterial, DeferredMaterial& material) {

		for (std::uint32_t j = 0; j < aiMaterial->GetTextureCount(type); j++) {

			aiString aiPath;
			aiMaterial->GetTexture(type, j, &aiPath);
			std::string path(directory);
			path.append("/");
			path.append(aiPath.C_Str());

			std::cout << "DEBUG::MODELLOADER::Loading texture '" << path << "' from type" << type << " \n";

			material.texturePaths.push_back({ path, TextureTypes::ofAiTextureType(type) });
		}
	}

	void ModelLoader::loadMaterialProperties(aiMaterial* aiMaterial, DeferredMaterial& material) {

		material.roughness = getMaterialFloat(aiMaterial, AI_MATKEY_SHININESS);
		material.metallic = getMaterialFloat(aiMaterial, AI_MATKEY_REFLECTIVITY);

		material.albedo = getMaterialColor(aiMaterial, AI_MATKEY_COLOR_DIFFUSE);
		material.specular = getMaterialColor(aiMaterial, AI_MATKEY_COLOR_SPECULAR);
		material.ambient = getMaterialColor(aiMaterial, AI_MATKEY_COLOR_AMBIENT);

	}

	float glib::ModelLoader::getMaterialFloat(aiMaterial* material, const char* property, int, int) {
		
		float value = 0;

		aiGetMaterialFloat(material, property, 0, 0, &value);

		return value;
	}

	glm::vec3 glib::ModelLoader::getMaterialColor(aiMaterial* material, const char* property, int, int) {
		
		aiColor4D specularColor;
		aiGetMaterialColor(material, property, 0, 0, &specularColor);

		return glm::vec3(specularColor.r, specularColor.g, specularColor.b);
	}

	void ModelLoader::expandNode(aiNode* node) {

		for (std::size_t i = 0; i < node->mNumChildren; i++)
			examinNode(node->mChildren[i]);
	}

	// workaround
	bool ModelLoader::isLoaded() {
		std::future_status status = worker.wait_for(std::chrono::seconds(0));
		return status == std::future_status::ready;
	}

	ModelData ModelLoader::getData() const {

		worker.wait();

		return ModelDataConverter::convert(data);
	}

	std::string ModelLoader::getFilePath() const {
		return filePath;
	}

}