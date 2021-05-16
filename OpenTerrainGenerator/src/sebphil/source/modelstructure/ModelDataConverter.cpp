#include "modelstructure/ModelDataConverter.h"

namespace otg {

	TextureRegister ModelDataConverter::texRegister;

	ModelData ModelDataConverter::convert(const DeferredModelData& deferredData) {

		ModelData modelData;

		convertMeshes(deferredData, modelData);

		return modelData;
	}

	void ModelDataConverter::convertMeshes(const DeferredModelData& deferredModelData, ModelData& modelData) {

		for (const DeferredMeshData& deferredMeshData : deferredModelData.meshes) {

			MeshData meshData{ deferredMeshData.vertices, deferredMeshData.indices };
			meshData.material = convertMaterial(deferredMeshData.material);

			convertTextures(deferredMeshData, meshData);

			modelData.meshes.push_back(meshData);
		}
	}

	Material ModelDataConverter::convertMaterial(const DeferredMaterial& deferredMaterial) {

		Material material;

		material.setRoughness(deferredMaterial.roughness);
		material.setMetallic(deferredMaterial.metallic);
		material.setOcclusion(deferredMaterial.occlusion);

		material.setAlbedo(deferredMaterial.albedo);
		material.setSpecular(deferredMaterial.specular);
		material.setAmbient(deferredMaterial.ambient);

		return material;
	}

	void otg::ModelDataConverter::convertTextures(const DeferredMeshData& deferredMeshData, MeshData& meshData) {

		for (const DeferredTexture& deferredTexture : deferredMeshData.texturePaths) {

			std::shared_ptr<TextureImage> image = texRegister.getTexture(deferredTexture.filePath, deferredTexture.type);
			meshData.material.addTexture(image);
		}
	}

}