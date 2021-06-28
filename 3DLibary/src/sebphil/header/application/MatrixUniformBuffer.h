#pragma once
#include "globjects/UniformBuffer.h"
#include "modelstructure/Model.h"
#include "camera/Camera.h"

namespace glib {

	class MatrixUniformBuffer {

	public:
		MatrixUniformBuffer();

		void setModelMatrices(const glib::Model& model);
		void setWorldMatrix(glm::mat4 worldMat);
		void setNormalMatrix(glm::mat4 normalMat);

		void setCamMatrices(const glib::Camera& cam);
		void setViewMatrix(glm::mat4 viewMat);
		void setProjectionMatrix(glm::mat4 projectionMat);

		void bindTo(std::uint32_t bindingPoint);

	private:
		UniformBuffer ubo;

	};

}