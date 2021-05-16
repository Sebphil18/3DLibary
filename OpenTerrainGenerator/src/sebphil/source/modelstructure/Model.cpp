#include "modelstructure/Model.h"
#include "stb/stb_image.h"
#include "glad/glad.h"
#include "modelstructure/Mesh.h"

namespace otg {

	Model::Model() noexcept : 
		worldMatrix(glm::mat4(1))
	{
	}

	Model::Model(const ModelData& data) noexcept  :
		worldMatrix(glm::mat4(1))
	{
		constructMeshes(data);
	}

	void Model::setData(const ModelData& data) {
		
		meshes.clear();

		constructMeshes(data);
	}

	void Model::constructMeshes(const ModelData& data) {
		for (const auto& meshData : data.meshes)
			meshes.emplace_back(meshData);
	}

	void Model::draw(ShaderProgram& program) {

		for (Mesh& mesh : meshes)
			mesh.draw(program);
	}

	void Model::setPosition(glm::vec3 position) {

		properties.position = position;
		transforms.translation = glm::translate(glm::mat4(1), position);
		updateWorldMatrix();
	}

	void Model::setScale(glm::vec3 scale) {

		properties.scale = scale;
		transforms.scale = glm::scale(glm::mat4(1), scale);
		updateWorldMatrix();
	}

	/// <param name="rotation">for x-, y-, z-axis in radians</param>
	void Model::setRotation(glm::vec3 rotation) {

		properties.rotation = rotation;
		transforms.rotationQuat = glm::quat(rotation);
		updateWorldMatrix();
	}

	void Model::updateWorldMatrix() {
		glm::mat4 rotation = glm::toMat4(transforms.rotationQuat);
		worldMatrix = transforms.translation * rotation * transforms.scale;
	}

	glm::mat4 Model::getWorldMatrix() const {
		return worldMatrix;
	}

	glm::mat4 Model::getNormalMatrix() const {
		return glm::transpose(glm::inverse(worldMatrix));
	}

}