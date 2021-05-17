#pragma once
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glad/glad.h"
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
		glm::quat rotationQuat = glm::quat(glm::vec3(0, 0, 0));
	};

	class Model {

	public:
		std::vector<Mesh> meshes;

		Model() noexcept;
		Model(const ModelData& data, std::uint32_t usage = GL_STATIC_DRAW) noexcept;

		void draw(ShaderProgram& program);

		void setData(const ModelData& data, std::uint32_t usage = GL_STATIC_DRAW);

		void setPosition(glm::vec3 position);
		void setScale(glm::vec3 scale);
		void setRotation(glm::vec3 rotation);

		glm::mat4 getWorldMatrix() const;
		glm::mat4 getNormalMatrix() const;

	private:
		WorldProperties properties;
		Transformations transforms;

		glm::mat4 worldMatrix;

		void constructMeshes(const ModelData& data, std::uint32_t usage = GL_STATIC_DRAW);
		void updateWorldMatrix();

	};

}