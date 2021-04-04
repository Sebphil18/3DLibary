#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "modelstructure/Mesh.h"
#include "globjects/TextureImage.h"

namespace otg {

	struct ModelData {
		std::vector<MeshData> meshes;
	};

	struct DeferredModelData {
		std::vector<DeferredMeshData> meshes;
	};

	struct WorldProperties {
		glm::vec3 position = glm::vec3(0);
		glm::vec3 rotation = glm::vec3(0);
		glm::vec3 scale = glm::vec3(1);
	};

	struct Transformations {
		glm::mat4 translation = glm::mat4(1);
		glm::mat4 scale = glm::mat4(1);
		glm::quat rotationQuat;
	};

	class Model {

	public:
		std::vector<Mesh> meshes;

		Model() noexcept;
		Model(const ModelData& data) noexcept;

		void draw(ShaderProgram& program);

		void setData(const ModelData& data);

		void addTexture(const std::shared_ptr<Texture>& texture, std::size_t meshIndex);
		void clearTextures(std::size_t meshIndex);

		void setPosition(glm::vec3 position);
		void setScale(glm::vec3 scale);
		void setRotation(glm::vec3 rotation);

		glm::mat4 getWorldMatrix() const;
		glm::mat4 getNormalMatrix() const;

	private:
		WorldProperties properties;
		Transformations transforms;

		glm::mat4 worldMatrix;

		void constructMeshes(const ModelData& data);
		void updateWorldMatrix();

	};

}