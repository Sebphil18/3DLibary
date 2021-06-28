#pragma once
#include <string>
#include <mutex>
#include <future>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/glm.hpp"
#include "modelstructure/Model.h"
#include "modelstructure/TextureRegister.h"
#include "globjects/TextureImage.h"

namespace glib {

	class ModelLoader {

	public:
		ModelLoader(const std::string& filePath);

		bool isLoaded();

		ModelData getData() const;
		std::string getFilePath() const;

	private:
		std::string directory;
		std::string fileName;
		std::string filePath;

		const aiScene* scene;

		std::mutex loadingLock;
		std::future<void> worker;

		DeferredModelData data;
		std::unordered_map<std::string, std::shared_ptr<glib::TextureImage>> loadedTextures;

		void resolveFilepath();

		// executed asynchronously
		void loadModel();
		void tryLoadModel(const Assimp::Importer& importer);

		void examinNode(aiNode* node);
		void loadMeshes(aiNode* node);
		void expandNode(aiNode* node);

		std::vector<Vertex> loadVertices(aiMesh* mesh);
		Vertex getVertex(aiMesh* mesh, std::uint32_t vertexIndex);

		glm::vec3 getPosition(std::uint32_t vertexIndex, aiMesh* mesh);
		glm::vec3 getNormal(std::uint32_t vertexIndex, aiMesh* mesh);
		void setTangents(std::uint32_t vertexIndex, aiMesh* mesh, Vertex& vertex);
		glm::vec3 getTangent(std::uint32_t vertexIndex, aiMesh* mesh);
		glm::vec3 getBitangent(std::uint32_t vertexIndex, aiMesh* mesh);
		glm::vec2 getTexCoords(std::uint32_t vertexIndex, aiMesh* mesh);

		static glm::vec3 convertVector(aiVector3D aiVec3);
		static glm::vec2 convertVector(aiVector2D aiVec2);

		std::vector<std::uint32_t> loadIndices(aiMesh* mesh);

		void loadMaterial(aiMesh* mesh, DeferredMaterial& material);
		void loadTextures(aiTextureType type, aiMaterial* material, DeferredMaterial& data);

		void loadMaterialProperties(aiMaterial* aiMaterial, DeferredMaterial& material);
		
		// Please note that the last two arguments don't do anything. They need to exist because AI_MATKEY_XXXX extends to 'const char*, uint, uint'
		// (e.g. "some text", 0, 0) and enable therefore the support for using assimp's macros.
		float getMaterialFloat(aiMaterial* material, const char* property, int, int);
		glm::vec3 getMaterialColor(aiMaterial* material, const char* property, int, int);

	};

}