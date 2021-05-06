#include "camera/Camera.h"
#include "glm/gtx/transform.hpp"

namespace otg {

	Camera::Camera() : 
		position(glm::vec3(0, 0, 5)),
		target(glm::vec3(0, 0, 0)),
		fov(65), near(0.01), far(500),
		movementSpeed(1), sensitivity(0.5),
		dimensions(1200, 800)
	{
		init();
	}

	Camera::Camera(glm::ivec2 dimensions) :
		position(glm::vec3(0, 0, 5)),
		target(glm::vec3(0, 0, 0)),
		fov(65), near(0.01), far(500),
		movementSpeed(1), sensitivity(0.5),
		dimensions(dimensions)
	{
		init();
	}

	Camera::Camera(glm::ivec2 dimensions, glm::vec3 position) :
		position(position),
		target(glm::vec3(0, 0, 0)),
		fov(65), near(0.01), far(500),
		movementSpeed(1), sensitivity(0.5),
		dimensions(dimensions)
	{
		init();
	}

	void Camera::init() {
		updateViewMatrix();
		updateProjection();
	}

	void Camera::setPosition(glm::vec3 position) {

		this->position = position;
		
		updateViewMatrix();
	}

	void Camera::setTarget(glm::vec3 target) {

		this->target = target;
		
		updateViewMatrix();
	}

	void Camera::moveLeft(float frameTime) {
		moveRight(-frameTime);
	}

	void Camera::moveRight(float frameTime) {

		float delta = movementSpeed * frameTime;
		glm::vec3 deltaPos = right * delta;

		this->position += deltaPos;

		updateViewMatrix();
	}

	void Camera::moveDown(float frameTime) {
		moveUp(-frameTime);
	}

	void Camera::moveUp(float frameTime) {

		float delta = movementSpeed * frameTime;
		glm::vec3 deltaPos = up * delta;

		this->position += deltaPos;

		updateViewMatrix();
	}

	void Camera::moveBackwards(float frameTime) {
		moveForward(-frameTime);
	}

	void Camera::moveForward(float frameTime) {

		float delta = movementSpeed * frameTime;
		glm::vec3 forward = target - position;
		glm::vec3 deltaPos = forward * delta;

		this->position += deltaPos;

		updateViewMatrix();
	}

	void Camera::updateViewMatrix() {
		constructAxis();
		view = glm::lookAt(position, target, up);
	}

	void Camera::constructAxis() {

		glm::vec3 yAxis(0, 1, 0);
		glm::vec3 forward = glm::normalize(position - target);

		right = glm::cross(yAxis, forward);
		up = glm::cross(forward, right);
	}

	void Camera::setFov(float fov) {

		this->fov = fov;

		updateProjection();
	}

	void Camera::setWidth(int width) {

		this->dimensions.x = width;

		updateProjection();
	}

	void Camera::setHeight(int height) {

		this->dimensions.y = height;

		updateProjection();
	}

	void Camera::setSize(int width, int height) {

		this->dimensions = { width, height };

		updateProjection();
	}

	void Camera::setSize(glm::ivec2 dimensions) {

		this->dimensions = dimensions;

		updateProjection();
	}

	void Camera::updateProjection() {

		if (dimensions.y == 0)
			return;

		float aspectratio = static_cast<float>(dimensions.x) / dimensions.y;

		projection = glm::perspective(glm::radians(fov), aspectratio, near, far);
	}

	void Camera::setMovementSpeed(float movementSpeed) {
		this->movementSpeed = movementSpeed;
	}

	void Camera::setSensitivity(float sensitivity) {
		this->sensitivity = sensitivity;
	}

	glm::vec3 Camera::getPosition() const {
		return position;
	}

	glm::vec3 Camera::getTarget() const {
		return target;
	}

	glm::mat4 Camera::getViewMatrix() const {
		return view;
	}

	glm::mat4 Camera::getProjectionMatrix() const{
		return projection;
	}

}