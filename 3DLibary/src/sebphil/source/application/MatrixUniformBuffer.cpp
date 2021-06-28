#include "application/MatrixUniformBuffer.h"
#include "glm/gtc/type_ptr.hpp"

namespace glib {

	MatrixUniformBuffer::MatrixUniformBuffer() {

		ubo.addElement({ UniformType::Matrix4 });
		ubo.addElement({ UniformType::Matrix4 });
		ubo.addElement({ UniformType::Matrix4 });
		ubo.addElement({ UniformType::Matrix4 });
	}

	void MatrixUniformBuffer::setModelMatrices(const glib::Model& model) {
		setWorldMatrix(model.getWorldMatrix());
		setNormalMatrix(model.getNormalMatrix());
	}

	void MatrixUniformBuffer::setWorldMatrix(glm::mat4 worldMat) {
		ubo.setElementData(0, glm::value_ptr(worldMat));
	}

	void MatrixUniformBuffer::setNormalMatrix(glm::mat4 normalMat) {
		ubo.setElementData(1, glm::value_ptr(normalMat));
	}

	void MatrixUniformBuffer::setCamMatrices(const glib::Camera& cam) {
		setViewMatrix(cam.getViewMatrix());
		setProjectionMatrix(cam.getProjectionMatrix());
	}

	void MatrixUniformBuffer::setViewMatrix(glm::mat4 viewMat) {
		ubo.setElementData(2, glm::value_ptr(viewMat));
	}

	void MatrixUniformBuffer::setProjectionMatrix(glm::mat4 projectionMat) {
		ubo.setElementData(3, glm::value_ptr(projectionMat));
	}

	void MatrixUniformBuffer::bindTo(std::uint32_t bindingPoint) {
		ubo.bindTo(bindingPoint);
	}

}