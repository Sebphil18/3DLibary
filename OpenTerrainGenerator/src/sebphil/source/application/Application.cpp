#include "application/Application.h"

#include <iostream>
#include "GLFW/glfw3.h"
#include "exceptions/GlfwException.h"
#include "exceptions/ApplicationException.h"

std::uint32_t otg::Application::instances = 0;

otg::Application::Application() {

	try {

		instantiate();
		initialize();

	} catch (otg::ApplicationException& exception) {
		std::cout << exception.what() << "\n";
	}
}

void otg::Application::instantiate() {

	instances++;

	if (instances > 1)
		throw otg::ApplicationException("Application already created!");
}

void otg::Application::initialize() {

	if (glfwInit())
		glfwSetErrorCallback(Application::errorCallback);
	else
		throw otg::ApplicationException("Could not initilize glfw!");
}

void otg::Application::errorCallback(int errorCode, const char* description) {

	otg::GlfwExcpetion exception(errorCode, description);
	std::cout << exception.what() << "\n";
}

otg::Application::~Application() {

	if (instances == 1)
		glfwTerminate();

	instances--;
}