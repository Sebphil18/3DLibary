#include "application/Application.h"

#include <iostream>
#include "GLFW/glfw3.h"
#include "exceptions/GlfwException.h"
#include "exceptions/ApplicationException.h"

std::uint32_t glib::Application::instances = 0;

glib::Application::Application() {

	try {

		instantiate();
		init();

	} catch (glib::ApplicationException& exception) {
		std::cout << exception.what() << "\n";
	}
}

void glib::Application::instantiate() {

	instances++;

	if (instances > 1)
		throw glib::ApplicationException("Application already created!");
}

void glib::Application::init() {

	if (glfwInit())
		glfwSetErrorCallback(Application::errorCallback);
	else
		throw glib::ApplicationException("Could not initilize glfw!");
}

void glib::Application::errorCallback(int errorCode, const char* description) {

	glib::GlfwExcpetion exception(errorCode, description);
	std::cout << exception.what() << "\n";
}

glib::Application::~Application() {

	if (instances == 1)
		glfwTerminate();

	instances--;
}