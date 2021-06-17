#pragma once
#include <iostream>
#include "camera/OrbitCamera.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/polar_coordinates.hpp"

namespace otg {
	
	OrbitCamera::OrbitCamera() :
		angles(0, 0), radius(5), middlePoint(0),
		Camera()
	{
	}

	OrbitCamera::OrbitCamera(glm::ivec2 dimensions) : 
		angles(0, 0), radius(5), middlePoint(0),
		Camera(dimensions)
	{
	}

	OrbitCamera::OrbitCamera(glm::ivec2 dimensions, glm::vec2 angles) :
		angles(angles), radius(5), middlePoint(0),
		Camera(dimensions)
	{
	}

	void OrbitCamera::setPosition(glm::vec3 position) {

		this->position = position;

		glm::vec3 sphericalPos = glm::polar(position);
		angles = { sphericalPos.x, sphericalPos.y };
		radius = sphericalPos.z;

		updateViewMatrix();
	}

	void OrbitCamera::setPosition(glm::vec2 angles, float radius) {

		this->angles = angles;
		this->radius = radius;
		
		updatePosition();
	}

	void OrbitCamera::setMiddlePoint(glm::vec3 middlePoint) {

		this->middlePoint = middlePoint;

		setTarget(middlePoint);
		updatePosition();
	}

	void OrbitCamera::move(glm::vec2 offset) {

		moveRight(offset.x * sensitivity);
		moveUp(offset.y * sensitivity);

		updatePosition();
	}

	void OrbitCamera::moveRight(float frameTime) {

		angles.y += movementSpeed * frameTime;
		updatePosition();
	}

	void OrbitCamera::moveUp(float frameTime) {

		float deltaAngle = movementSpeed * frameTime;
		float newAngle = angles.x + deltaAngle;

		constexpr float half_pi = glm::half_pi<float>();
		bool camFacingDown = newAngle > half_pi - 0.1;
		bool camFacingUp = newAngle < -half_pi + 0.05;

		if (!camFacingDown && !camFacingUp)
			angles.x = newAngle;

		updatePosition();
	}

	void OrbitCamera::moveForward(float delta) {

		float deltaRadius = delta * movementSpeed;
		float newRadius = radius + deltaRadius;

		if (newRadius >= 1)
			radius = newRadius;
		else
			radius = 1;

		updatePosition();
	}

	void OrbitCamera::updatePosition() {

		this->position = glm::euclidean(angles) * radius + middlePoint;
		updateViewMatrix();
	}

}