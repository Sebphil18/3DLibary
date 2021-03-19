#include <iostream>
#include <string>
#include <memory>

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
#include "globjects/ProgramUniformLink.h"
#include "globjects/TextureImage.h"
#include "globjects/Texture.h"
#include "globjects/MultisampleTexture.h"
#include "globjects/RenderBuffer.h"
#include "globjects/MultisampleRenderBuffer.h"
#include "globjects/Framebuffer.h"
#include "io/Image.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/Model.h"
#include "stb/stbimage.h"

// standard VertexFormat: position | normal | tangent | bitangent | textureCoordinates
static std::vector<otg::Vertex> screenVertices = {
	{ glm::vec3(-1, -0.1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) }
};

static std::vector<unsigned int> screenIndices = {
	0, 1, 3,
	1, 2, 3,
};

static otg::Mesh* meshPtr;
static otg::ShaderProgram* programPtr;

static std::vector<std::shared_ptr<otg::Model>> models;
static std::vector<std::shared_ptr<otg::ShaderProgram>> programs;

static void launchApp();
static void update(otg::FrameClock& clock);
static void draw();

/*
* UniformBuffer
* ModelLoader
*/

int main() {
	launchApp();
}

static void launchApp() {

	otg::Application app;

	otg::Window window("OpenTerrainGenerator");
	GLFWwindow* const glfwWindow = window.getGlfwWindow();

	otg::DebugMessenger debugMessenger;

	// ShaderPrograms
	std::shared_ptr<otg::ShaderProgram> screenProgram = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/VertexScreen.glsl", 
		"src/sebphil/shader/fragment/FragmentScreen.glsl"
		);
	programs.push_back(screenProgram);

	std::shared_ptr<otg::ShaderProgram> program = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/VertexStandard.glsl",
		"src/sebphil/shader/fragment/FragmentStandard.glsl"
		);
	programs.push_back(program);

	// Model
	std::shared_ptr<otg::TextureImage> texture = std::make_shared<otg::TextureImage>(
		"rec\\textures\\testtexture\\TestTexture.png",
		otg::TextureType::Albedo
		);

	otg::MeshData meshData;
	meshData.indices = screenIndices;
	meshData.vertices = screenVertices;
	meshData.material = { 0, 0 };
	meshData.textures.push_back(texture);

	otg::ModelData modelData;
	modelData.meshes.push_back(meshData);

	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>(modelData);
	models.push_back(model);

	// Framebuffer
	int width = window.getWidth();
	int height = window.getHeight();

	std::shared_ptr<otg::Texture> screenTexture = std::make_shared<otg::Texture>(width, height, otg::TextureType::ColorAttachment);
	otg::RenderBuffer renderBuffer(width, height, otg::TextureType::DepthStencilAttachment);

	otg::Framebuffer framebuffer;
	framebuffer.attachTexture(*screenTexture);
	framebuffer.attachRenderBuffer(renderBuffer);
	framebuffer.validate();

	// MULTISAMPLING
	otg::Framebuffer multiFramebuffer;
	otg::MultisampleTexture multiTexture(width, height);
	otg::MultisampleRenderBuffer multiRbo(width, height);

	multiFramebuffer.attachTexture(multiTexture);
	multiFramebuffer.attachRenderBuffer(multiRbo);
	multiFramebuffer.validate();

	multiFramebuffer.bind();
	model->draw(*program);
	multiFramebuffer.unbind();

	multiFramebuffer.copyColorTo(width, height, framebuffer);

	// update model-textures so it will display only the screenTexture
	model->clearTextures(0);
	model->addTexture(screenTexture, 0);

	otg::RenderLoop loop(glfwWindow);
	loop.setUpdateFunc(update);
	loop.setDrawFunc(draw);
	loop.start();
}

static void update(otg::FrameClock& clock) {
	clock.update();
}

static void draw() {

	glClearColor(1, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	models[0]->draw(*programs[0]);
}