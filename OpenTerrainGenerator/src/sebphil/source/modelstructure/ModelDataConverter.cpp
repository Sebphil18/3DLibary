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
			meshData.material = deferredMeshData.material;

			convertTextures(deferredMeshData, meshData);

			modelData.meshes.push_back(meshData);
		}
	}

	void otg::ModelDataConverter::convertTextures(const DeferredMeshData& deferredMeshData, MeshData& meshData) {

		for (const DeferredTexture& deferredTexture : deferredMeshData.texturePaths) {

			std::shared_ptr<TextureImage> image = texRegister.getTexture(deferredTexture.filePath, deferredTexture.type);
			meshData.textures.push_back(image);
		}
	}

}