#include <iostream>
#include <string>
#include <memory>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "application/Application.h"
#include "application/Window.h"
#include "application/RenderLoop.h"

#include "globjects/VertexBuffer.h"
#include "globjects/IndexBuffer.h"
#include "globjects/VertexArray.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/UniformBuffer.h"
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
#include "modelstructure/CubeData.h"

#include "io/Image.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/ScreenMesh.h"
#include "modelstructure/Model.h"
#include "camera/Camera.h"
#include "stb/stbimage.h"

static std::shared_ptr<otg::ScreenMesh> screen;
static otg::ShaderProgram* programPtr;

static std::vector<std::shared_ptr<otg::Model>> models;
static std::vector<std::shared_ptr<otg::ShaderProgram>> programs;

static void launchApp();

static void update(otg::FrameClock& clock);
static void draw();

/*
* TODO: add support for resizing Textures and RenderBuffers
* Camera
* ModelLoader
*/

int main() {
	launchApp();
}

static void launchApp() {

	otg::Application app;

	otg::Window window("OpenTerrainGenerator");
	int width = window.getWidth();
	int height = window.getHeight();

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

	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>();
	model->meshes.emplace_back(cubeVertices, cubeIndices);
	model->addTexture(texture, 0);
	model->setRotation(glm::vec3(0.3, 0.4, 0));
	models.push_back(model);

	// Matrices
	otg::Camera cam(width, height);
	cam.setPosition(glm::vec3(0, 0, 2));

	glm::mat4 world = model->getWorldMatrix();
	glm::mat4 normal = model->getNormalMatrix();
	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 projection = cam.getProjectionMatrix();

	otg::UniformBuffer ubo(4 * 64);
	ubo.addElement({ otg::UniformType::Matrix4, glm::value_ptr(world) });
	ubo.addElement({ otg::UniformType::Matrix4, glm::value_ptr(normal) });
	ubo.addElement({ otg::UniformType::Matrix4, glm::value_ptr(view) });
	ubo.addElement({ otg::UniformType::Matrix4, glm::value_ptr(projection) });
	ubo.bindTo(*program, "Matrices", 0);

	// Multisampling
	otg::MultisampleRenderBuffer multiRbo(width, height);
	otg::MultisampleTexture multiTex(width, height);
	otg::Framebuffer multiFbo;
	
	multiFbo.attachRenderBuffer(multiRbo);
	multiFbo.attachTexture(multiTex);
	multiFbo.validate();

	multiFbo.bind();
	multiFbo.clear();
	model->draw(*program);
	multiFbo.unbind();

	// Framebuffer
	std::shared_ptr<otg::Texture> screenTexture = std::make_shared<otg::Texture>(width, height, otg::TextureType::ColorAttachment);
	otg::RenderBuffer rbo(width, height, otg::TextureType::DepthStencilAttachment);
	otg::Framebuffer fbo;

	fbo.attachRenderBuffer(rbo);
	fbo.attachTexture(*screenTexture);
	fbo.validate();

	multiFbo.copyColorTo(width, height, fbo);

	// Setup screen
	screen = std::make_shared<otg::ScreenMesh>();
	screen->addTexture(screenTexture);

	// renderloop
	GLFWwindow* const glfwWindow = window.getGlfwWindow();

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

	screen->draw(*programs[0]);
}