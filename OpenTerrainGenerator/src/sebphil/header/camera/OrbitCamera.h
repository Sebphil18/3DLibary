#pragma once
#include "Camera.h"

namespace otg {

	class OrbitCamera : public Camera {

	public:
		OrbitCamera();
		OrbitCamera(glm::ivec2 dimensions);
		OrbitCamera(glm::ivec2 dimensions, glm::vec2 angles);

		void setPosition(glm::vec3 position) override;
		void setPosition(glm::vec2 angles, float radius);

		void setMiddlePoint(glm::vec3 middlePoint);

		void move(glm::vec2 offset) override;
		void moveRight(float frameTime) override;
		void moveUp(float frameTime) override;
		void moveForward(float delta) override;

	private:
		float radius;
		glm::vec2 angles;
		glm::vec3 middlePoint;

		void updatePosition();

	};

}