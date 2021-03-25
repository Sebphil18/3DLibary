#include "camera/Camera.h"
#include "glm/gtx/transform.hpp"

namespace otg {

	Camera::Camera() : 
		position(glm::vec3(0, 0, 5)),
		target(glm::vec3(0, 0, 0)),
		fov(65), near(0.01), far(500),
		width(800), height(800)
	{
		init();
	}

	Camera::Camera(std::uint32_t width, std::uint32_t height) :
		position(glm::vec3(0, 0, 5)),
		target(glm::vec3(0, 0, 0)),
		fov(65), near(0.01), far(500),
		width(width), height(height)
	{
		init();
	}

	Camera::Camera(std::uint32_t width, std::uint32_t height, glm::vec3 position) :
		position(position),
		target(glm::vec3(0, 0, 0)),
		fov(65), near(0.01), far(500),
		width(width), height(height)
	{
		init();
	}

	void Camera::init() {
		constructAxis();
		updateView();
		updateProjection();
	}

	void Camera::setPosition(glm::vec3 position) {

		this->position = position;
		
		constructAxis();
		updateView();
	}

	void Camera::setTarget(glm::vec3 target) {

		this->target = target;
		
		constructAxis();
		updateView();
	}

	void Camera::constructAxis() {

		glm::vec3 yAxis(0, 1, 0);
		glm::vec3 forward = glm::normalize(position - target);

		right = glm::cross(forward, yAxis);
		up = glm::cross(forward, right);
	}

	void Camera::updateView() {
		view = glm::lookAt(position, target, up);
	}

	void Camera::setFov(float fov) {

		this->fov = fov;

		updateProjection();
	}

	void Camera::setWidth(std::uint32_t width) {

		this->width = width;

		updateProjection();
	}

	void Camera::setHeight(std::uint32_t height) {

		this->height = height;

		updateProjection();
	}

	void Camera::updateProjection() {
		projection = glm::perspective(fov, (float)width / height, near, far);
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