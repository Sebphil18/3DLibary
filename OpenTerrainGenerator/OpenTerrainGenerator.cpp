#include <iostream>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include "application/Application.h"
#include "application/Window.h"
#include "application/RenderLoop.h"

#include "globjects/VertexBuffer.h"
#include "globjects/IndexBuffer.h"
#include "globjects/VertexArray.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/DebugMessenger.h"
#include "globjects/Shader.h"
#include "globjects/ShaderProgram.h"

static float screenVertices[2 * 4] = {
	-0.5, -0.5,
	0.5, -0.5,
	0.5, 0.5,
	-0.5, 0.5
};

static unsigned int screenIndices[2 * 3] = {
	0, 1, 3,
	1, 2, 3,
};

static void launchApp();
static void update(otg::FrameClock& clock);
static void draw();

/*
TODO: Add uniform support for shaderprogram
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

	otg::VertexBuffer vbo(sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	otg::IndexBuffer ibo(sizeof(screenIndices), screenIndices, GL_STATIC_DRAW);

	otg::VertexArray vao;
	vao.use();

	otg::VertexBufferLayout bufferLayout;
	bufferLayout.addElement({otg::ElementType::Float, 2, false});
	bufferLayout.applyLayout(vao.getGlHandle(), vbo.getGlHandle(), ibo.getGlHandle());

	otg::ShaderProgram program("src/sebphil/shader/vertex/VertexStandard.glsl", "src/sebphil/shader/fragment/FragmentScreen.glsl");
	program.use();

	loop.start();

}

static void update(otg::FrameClock& clock) {
	clock.update();
}

static void draw() {

	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}