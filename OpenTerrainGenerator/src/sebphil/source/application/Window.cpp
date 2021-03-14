#include "application/Window.h"
#include <iostream>
#include "exceptions/ApplicationException.h"
#include "debugflags/DebugFlags.h"

otg::Window::Window(const std::string& title, int width, int height):
	title(title), width(width), height(height)
{
	setWindowHints();
	generateWindow();

	setWindowCallbacks();

	setUpOpenGL();
}

void otg::Window::setWindowHints() {

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, SEB_DEBUG_MODE);

	glfwWindowHint(GLFW_DOUBLEBUFFER, true);
}

void otg::Window::generateWindow() {

	try {

		makeContext();

	} catch (ApplicationException& exception) {

		std::cout << exception.what() << "\n";
	}
}

void otg::Window::makeContext() {
	createWindow();
	glfwMakeContextCurrent(window);
	loadGlBindings();
}

void otg::Window::createWindow() {

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

	if (!window)
		throw ApplicationException("Could not create window: " + title);
}

void otg::Window::loadGlBindings() {

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw ApplicationException("Could not load OpenGL-Bindings (please be sure to bind a context first).");
}

void otg::Window::setWindowCallbacks() noexcept {
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void otg::Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept {
	glViewport(0, 0, width, height);
}

void otg::Window::setUpOpenGL() {
	glEnable(GL_DEPTH_TEST | GL_FRAMEBUFFER_SRGB | GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

otg::Window::~Window() {
	glfwDestroyWindow(window);
}

void otg::Window::focus() const noexcept {

	glfwMakeContextCurrent(window);
	glfwRequestWindowAttention(window);
}

int otg::Window::getWidth() const noexcept {

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	return width;
}

int otg::Window::getHeight() const noexcept {

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	return height;
}

GLFWwindow* const otg::Window::getGlfwWindow() const noexcept {
	return window;
}

void otg::Window::setFramebufferSizeCallback(GLFWframebuffersizefun func) {
	glfwSetFramebufferSizeCallback(window, func);
}