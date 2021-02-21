#include "application/RenderLoop.h"
#include <iostream>

otg::RenderLoop::RenderLoop(GLFWwindow* window) :
	window(window), active(false)
{
	update = standardUpdate;
	draw = standardDraw;
	processInput = standardInputProcessing;
}

void otg::RenderLoop::start() {

	active = true;
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window) && active)
		tick();
}

void otg::RenderLoop::tick() {

	update(clock);
	draw();

	glfwSwapBuffers(window);

	glfwPollEvents();
	processInput(window);

}

void otg::RenderLoop::standardUpdate(FrameClock& clock) {

	clock.update();
}

void otg::RenderLoop::standardDraw() {

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void otg::RenderLoop::standardInputProcessing(GLFWwindow* window) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

}

void otg::RenderLoop::stop() {
	active = false;
}

bool otg::RenderLoop::isRunning() const {
	return active;
}

void otg::RenderLoop::setUpdateFunc(const std::function<void(FrameClock&)>& function) {
	update = function;
}

void otg::RenderLoop::setDrawFunc(const std::function<void()>& function) {
	draw = function;
}

void otg::RenderLoop::setInputFunc(const std::function<void(GLFWwindow* window)>& function) {
	processInput = function;
}