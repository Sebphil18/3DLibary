#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

static void setUpGlfw();
static void errorCallback(int error, const char* msg);
static void initGlfw();
static void printGlfwInfo();
static GLFWwindow* setUpWindow();
static void loadGlBindings();
static void printGlInfo();

int main() {

	setUpGlfw();

	GLFWwindow* window = setUpWindow();

	loadGlBindings();
	printGlInfo();

	std::cout << "press any key to continue... \n";
	std::cin.get();

	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {

		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		double fps = 1 / delta;
		lastTime = currentTime;

		std::cout << "frametime: " << delta << "s \n";
		std::cout << "frameRate: " << fps << "fps \n";
		std::cout << "\n";

		glClearColor(1, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void setUpGlfw() {

	initGlfw();
	printGlfwInfo();

	glfwSetErrorCallback(errorCallback);
}

static void errorCallback(int error, const char* msg) {
	std::cout << "ERROR::GLFW::" << error << "::" << msg << "\n";
}

void initGlfw() {
	bool init = glfwInit();
	if (!init)
		std::cout << "ERROR::GLFW::Could not initilize glfw. \n";
}

void printGlfwInfo() {
	const char* glfwVersion = glfwGetVersionString();
	std::cout << "glfw_version: " << glfwVersion << "\n \n";
}

GLFWwindow* setUpWindow() {

	GLFWwindow* window = glfwCreateWindow(100, 100, "TestApplication", nullptr, nullptr);
	if (!window) {
		std::cout << "ERROR::GLFW::Could not create window! \n";
		return nullptr;
	}

	glfwMakeContextCurrent(window);

	return window;
}

void loadGlBindings() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "ERROR::GLAD::Could not load GL-loader! \n";
}

void printGlInfo() {

	const char* version = (char*)glGetString(GL_VERSION);
	const char* renderer = (char*)glGetString(GL_RENDERER);
	const char* glslVersion = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	std::cout << "version: " << version << "\n";
	std::cout << "renderer: " << renderer << "\n";
	std::cout << "shading language version: " << glslVersion << "\n";
}
