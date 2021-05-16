#pragma once
#include "modelstructure/Model.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/TextureRegister.h"

namespace otg {

	class ModelDataConverter {

	public:
		static ModelData convert(const DeferredModelData& deferredData);

	private:
		static TextureRegister texRegister;

		static void convertMeshes(const DeferredModelData& deferredModelData, ModelData& modelData);
		static Material convertMaterial(const DeferredMaterial& deferredMaterial);
		static void convertTextures(const DeferredMeshData& deferredMeshData, MeshData& meshData);

	};

}