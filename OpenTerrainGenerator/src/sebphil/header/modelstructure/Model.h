#pragma once
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "modelstructure/Mesh.h"
#include "globjects/TextureImage.h"

namespace otg {

	struct ModelData {
		std::vector<MeshData> meshes;
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
		Model() noexcept;
		Model(const ModelData& data) noexcept;

		void draw(ShaderProgram& program);

		void addMesh(const Mesh& mesh);

		void setPosition(glm::vec3 position);
		void setScale(glm::vec3 scale);
		void setRotation(glm::vec3 rotation);

		glm::mat4 getWorldMat() const;

	private:
		WorldProperties properties;
		Transformations transforms;

		std::vector<Mesh> meshes;

	};

}