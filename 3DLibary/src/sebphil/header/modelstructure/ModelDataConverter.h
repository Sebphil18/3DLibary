#pragma once
#include "modelstructure/Model.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/TextureRegister.h"

namespace glib {

	class ModelDataConverter {

	public:
		static ModelData convert(const DeferredModelData& deferredData);

	private:
		static TextureRegister texRegister;

		static std::vector<MeshData> convertMeshes(const DeferredModelData& deferredModelData);
		static Material convertMaterial(const DeferredMaterial& deferredMaterial);
		static std::vector<std::shared_ptr<Texture>> convertTextures(const DeferredMaterial& material);

	};

}