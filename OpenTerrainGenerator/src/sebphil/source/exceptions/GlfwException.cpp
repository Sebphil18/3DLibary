#include "exceptions/GlfwException.h"

#include <iostream>
#include "GLFW/glfw3.h"

otg::GlfwExcpetion::GlfwExcpetion(int errorCode, const char* description) {
	message = "ERROR::GLFW::" + std::to_string(errorCode) + ": " + description;
}

const char* otg::GlfwExcpetion::what() const noexcept {
	return message.c_str();
}