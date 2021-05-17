#include "modelstructure/ModelDataConverter.h"

namespace otg {

	TextureRegister ModelDataConverter::texRegister;

	ModelData ModelDataConverter::convert(const DeferredModelData& deferredData) {

		ModelData modelData;
		modelData.meshes = convertMeshes(deferredData);

		return modelData;
	}

	std::vector<MeshData> ModelDataConverter::convertMeshes(const DeferredModelData& deferredModelData) {

		std::vector<MeshData> meshes;

		for (const DeferredMeshData& deferredMeshData : deferredModelData.meshes) {

			MeshData meshData{ deferredMeshData.vertices, deferredMeshData.indices };
			meshData.material = convertMaterial(deferredMeshData.material);

			meshes.push_back(meshData);
		}

		return meshes;
	}

	Material ModelDataConverter::convertMaterial(const DeferredMaterial& deferredMaterial) {

		Material material;

		material.setRoughness(deferredMaterial.roughness);
		material.setMetallic(deferredMaterial.metallic);
		material.setOcclusion(deferredMaterial.occlusion);

		material.setAlbedo(deferredMaterial.albedo);
		material.setSpecular(deferredMaterial.specular);
		material.setAmbient(deferredMaterial.ambient);

		material.setTextures(convertTextures(deferredMaterial));

		return material;
	}

	std::vector<std::shared_ptr<Texture>> ModelDataConverter::convertTextures(const DeferredMaterial& material) {

		std::vector<std::shared_ptr<Texture>> textures;

		for (const DeferredTexture& deferredTexture : material.texturePaths) {

			std::shared_ptr<TextureImage> image = texRegister.getTexture(deferredTexture.filePath, deferredTexture.type);
			textures.push_back(image);
		}

		return textures;
	}

}