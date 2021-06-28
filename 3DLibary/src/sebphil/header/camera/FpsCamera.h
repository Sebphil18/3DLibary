#pragma once
#include "Camera.h"

namespace glib {

	class FpsCamera : public Camera {

	public:
		FpsCamera();
		FpsCamera(glm::ivec2 dimensions);
		FpsCamera(glm::ivec2 dimensions, glm::vec3 position);
		
		void move(glm::vec2 offsets) override;
		void moveForward(float frameTime) override;

	private:
		glm::vec2 angles;
		glm::vec3 forward;

		void updateViewMatrix() override;
		void constructAxis() override;

	};

}