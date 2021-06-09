#include "application/Mouse.h"
#include <iostream>

namespace otg {

	Mouse::Mouse(Window& window) :
		windowHandle(window.getGlfwWindow()), 
		lastPos(-1), delta(0), lastScrollPos(0), scrollDelta(0)
	{
		window.setScrollCallback([&](GLFWwindow* window, double xoffset, double yoffset) {
			scrollDelta = glm::ivec2(xoffset, -yoffset);
			lastScrollPos += scrollDelta;
			});
	}

	void Mouse::move() {
		glm::dvec2 newPos = getNewPos();
		updatePosDelta(newPos);
		lastPos = newPos;
	}

	glm::vec2 Mouse::getNewPos() {

		glm::dvec2 newPos(0);
		glfwGetCursorPos(windowHandle, &newPos.x, &newPos.y);
		newPos.y *= -1;

		return newPos;
	}

	void Mouse::updatePosDelta(glm::dvec2 newPos) {

		if (lastPos.x != -1)
			delta = lastPos - newPos;
	}

	void Mouse::resetScrollDelta() {
		scrollDelta = glm::ivec2(0);
	}

	glm::ivec2 Mouse::getScrollOffset() const {
		return scrollDelta;
	}

	std::int32_t Mouse::getScrollOffsetY() const {
		return scrollDelta.y;
	}

	glm::dvec2 Mouse::getLastPos() const {
		return lastPos;
	}

	glm::dvec2 Mouse::getPosDelta() const {
		return delta;
	}

	bool Mouse::isPrimaryPressed() {
		return glfwGetMouseButton(windowHandle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;
	}

	bool Mouse::isButtonPressed(int button) {
		return glfwGetMouseButton(windowHandle, button) == GLFW_PRESS;
	}

}