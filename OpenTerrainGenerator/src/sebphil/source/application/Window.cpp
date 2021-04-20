#include "application/Window.h"
#include <iostream>
#include "exceptions/ApplicationException.h"
#include "debugflags/DebugFlags.h"

namespace otg {

	std::unordered_map<GLFWwindow*, WindowSizeCallback> Window::sizeCallbacks;
	std::unordered_map<GLFWwindow*, WindowInputCallback> Window::inputCallbacks;
	std::unordered_map<GLFWwindow*, WindowCoursorPosCallback> Window::coursorPosCallbacks;

	Window::Window(const std::string& title, int width, int height) :
		title(title), width(width), height(height) {
		setWindowHints();
		generateWindow();

		setWindowCallbacks();

		setUpOpenGL();
	}

	void Window::setWindowHints() {

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, SEB_DEBUG_MODE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		glfwWindowHint(GLFW_DOUBLEBUFFER, true);
	}

	void Window::generateWindow() {

		try {

			makeContext();

		} catch (ApplicationException& exception) {

			std::cout << exception.what() << "\n";
		}
	}

	void Window::makeContext() {
		createWindow();
		glfwMakeContextCurrent(window);
		loadGlBindings();
	}

	void Window::createWindow() {

		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!window)
			throw ApplicationException("Could not create window: " + title);
	}

	void Window::loadGlBindings() {

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			throw ApplicationException("Could not load OpenGL-Bindings (please be sure to bind a context first).");
	}

	void Window::setWindowCallbacks() noexcept {

		glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetCursorPosCallback(window, coursorPosCallback);
	}

	void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept {

		auto itr = sizeCallbacks.find(window);

		if (itr != sizeCallbacks.end()) {

			WindowSizeCallback callback = itr->second;
			callback(window, width, height);
		}
	}

	void Window::keyCallback(GLFWwindow* window, int  key, int scancode, int action, int mods) {

		auto itr = inputCallbacks.find(window);

		if (itr != inputCallbacks.end()) {

			WindowInputCallback callback = itr->second;
			callback(window, key, scancode, action, mods);
		}
	}

	void Window::coursorPosCallback(GLFWwindow* window, double xpos, double ypos) {

		auto itr = coursorPosCallbacks.find(window);

		if (itr != coursorPosCallbacks.end()) {

			WindowCoursorPosCallback callback = itr->second;
			callback(window, xpos, ypos);
		}
	}

	void Window::setUpOpenGL() {

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	Window::~Window() {

		glfwDestroyWindow(window);
		removeCallbacks();
	}

	void Window::removeCallbacks() {

		sizeCallbacks.erase(window);
		inputCallbacks.erase(window);
		coursorPosCallbacks.erase(window);
	}

	void Window::focus() const noexcept {

		glfwMakeContextCurrent(window);
		glfwRequestWindowAttention(window);
	}

	int Window::getWidth() const noexcept {

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		return width;
	}

	int Window::getHeight() const noexcept {

		int width, height;
		glfwGetWindowSize(window, &width, &height);

		return height;
	}

	GLFWwindow* const Window::getGlfwWindow() const noexcept {
		return window;
	}

	void Window::setSizeCallback(const WindowSizeCallback& callback) {
		sizeCallbacks[window] = callback;
	}

	void Window::setKeyCallback(const WindowInputCallback& callback) {
		inputCallbacks[window] = callback;
	}

	void Window::setCoursorPosCallback(const WindowCoursorPosCallback& callback) {
		coursorPosCallbacks[window] = callback;
	}

	bool Window::shouldClose() const {
		return glfwWindowShouldClose(window);
	}
}