#include "modelstructure/Model.h"
#include "stb/stb_image.h"
#include "glad/glad.h"

namespace otg {

	Model::Model() noexcept {
	}

	Model::Model(const ModelData& data) noexcept {

		for (const auto& meshData : data.meshes)
			meshes.push_back(meshData);
	}

	void Model::draw(ShaderProgram& program) {

		for (Mesh& mesh : meshes)
			mesh.draw(program);
	}

	void Model::addMesh(const Mesh& mesh) {
		meshes.push_back(mesh);
	}

	void Model::setPosition(glm::vec3 position) {

		properties.position = position;
		transforms.translation = glm::translate(glm::mat4(1), position);
	}

	void Model::setScale(glm::vec3 scale) {

		properties.scale = scale;
		transforms.scale = glm::scale(glm::mat4(1), scale);
	}

	/// <param name="rotation">for x-, y-, z-axis in radians</param>
	void Model::setRotation(glm::vec3 rotation) {

		properties.rotation = rotation;
		transforms.rotationQuat = glm::quat(rotation);
	}

	glm::mat4 Model::getWorldMat() const {

		glm::mat4 rotation = glm::toMat4(transforms.rotationQuat);
		glm::mat4 worldMat = rotation * transforms.translation * transforms.scale;

		return worldMat;
	}

}