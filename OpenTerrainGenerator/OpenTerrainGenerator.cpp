#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"

#include "application/Application.h"
#include "application/Window.h"
#include "application/RenderLoop.h"

#include "globjects/VertexBuffer.h"
#include "globjects/IndexBuffer.h"
#include "globjects/VertexArray.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/DebugMessenger.h"

static float screenVertices[3 * 4] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1
};

static unsigned int screenIndices[3 * 2] = {
	0, 1, 3,
	1, 2, 3
};

static void launchApp();
static void update(otg::FrameClock& clock);
static void draw();

/*
* TODO: Unittests
* TODO: ShaderProgram
* TODO: Shader
* TODO: Mesh
* TODO: Model
* TODO: Texture
* TODO: Image
* TODO: IndexBuffer
* TODO: UniformBufferObject
* TODO: Camera
*/

int main() {
	launchApp();
}

static void launchApp() {

	otg::Application app;

	otg::Window window("OpenTerrainGenerator");
	GLFWwindow* const glfwWindow = window.getGlfwWindow();

	otg::DebugMessenger debugMessenger;

	otg::RenderLoop loop(glfwWindow);
	loop.setUpdateFunc(update);
	loop.setDrawFunc(draw);

	otg::VertexBuffer vbo;
	vbo.setData(sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

	otg::IndexBuffer ibo;
	ibo.setData(sizeof(unsigned int) * 6, screenIndices, GL_STATIC_DRAW);

	otg::VertexArray vao;

	otg::VertexBufferLayout layout;
	layout.addElement({otg::ElementType::Float, 3, false});
	layout.applyLayout(vao.getGlHandle(), vbo.getGlHandle(), ibo.getGlHandle());

	loop.start();

}

static void update(otg::FrameClock& clock) {
	clock.update();
}

static void draw() {

	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}