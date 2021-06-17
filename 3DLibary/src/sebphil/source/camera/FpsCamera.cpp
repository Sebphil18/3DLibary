#include "camera/FpsCamera.h"
#include "glm/gtx/polar_coordinates.hpp"
#include "glm/gtx/transform.hpp"

namespace otg {

	FpsCamera::FpsCamera() :
		angles(0, 0), forward(0, 0, -1),
		Camera()
	{
		sensitivity = 0.005;
		movementSpeed = 2;
	}

	FpsCamera::FpsCamera(glm::ivec2 dimensions) :
		angles(0, 0), forward(0, 0, -1),
		Camera(dimensions) 	
	{
		sensitivity = 0.005;
		movementSpeed = 2;
	}

	FpsCamera::FpsCamera(glm::ivec2 dimensions, glm::vec3 position) :
		angles(0, 0),
		Camera(dimensions, position) 	
	{
		forward = glm::normalize(-position);
		sensitivity = 0.005;
		movementSpeed = 2;
	}

	void FpsCamera::moveForward(float frameTime) {

		float delta = movementSpeed * frameTime;
		glm::vec3 deltaPosition = forward * delta;

		position += deltaPosition;

		updateViewMatrix();
	}

	void FpsCamera::move(glm::vec2 offsets) {

		angles.x += offsets.y * sensitivity;
		angles.y += offsets.x * sensitivity;

		angles.x = glm::clamp(angles.x, -glm::half_pi<float>() + 0.01f, glm::half_pi<float>() - 0.01f);

		updateViewMatrix();
	}

	void FpsCamera::updateViewMatrix() {

		constructAxis();

		target = position + forward;

		view = glm::lookAt(position, target, up);
	}

	void FpsCamera::constructAxis() {

		up = glm::vec3(0, 1, 0);
		forward = -glm::euclidean(angles);
		right = glm::normalize(glm::cross(forward, up));
	}

}