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
#include "io/Image.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/Model.h"
#include "stb/stbimage.h"

static std::vector<otg::Vertex> screenVertices = {
	{ glm::vec3(-1, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },
	{ glm::vec3(1, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(1, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(-1, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) }
};

static std::vector<unsigned int> screenIndices = {
	0, 1, 3,
	1, 2, 3,
};

static otg::Mesh* meshPtr;
static otg::ShaderProgram* programPtr;

static std::vector<std::shared_ptr<otg::Mesh>> meshes;
static std::vector<std::shared_ptr<otg::ShaderProgram>> programs;

static void launchApp();
static void update(otg::FrameClock& clock);
static void draw();

/*
* ImageLoader class (use buffer as shared_ptr and define custom deallocator)
* Texture support for Mesh
* UniformBuffer
* Model
*/

int main() {
	launchApp();
}

static void launchApp() {

	otg::Application app;

	otg::Window window("OpenTerrainGenerator");
	GLFWwindow* const glfwWindow = window.getGlfwWindow();

	otg::DebugMessenger debugMessenger;

	std::shared_ptr<otg::ShaderProgram> program = std::make_shared<otg::ShaderProgram>("src/sebphil/shader/vertex/VertexStandard.glsl", "src/sebphil/shader/fragment/FragmentScreen.glsl");
	programs.push_back(program);

	std::shared_ptr<otg::Mesh> mesh = std::make_shared<otg::Mesh>(screenVertices, screenIndices);
	std::shared_ptr<otg::TextureImage> texture = std::make_shared<otg::TextureImage>("rec\\textures\\testtexture\\TestTexture.png", otg::TextureType::Albedo);
	mesh->addTexture(texture);

	meshes.push_back(mesh);

	otg::MeshData meshData;
	meshData.indices = screenIndices;
	meshData.vertices = screenVertices;
	meshData.textures = std::vector<std::shared_ptr<otg::TextureImage>>();
	meshData.material = { 0, 0 };

	otg::ModelData modelData;
	modelData.meshes = std::vector<otg::MeshData>();
	modelData.meshes.push_back(meshData);

	otg::Model model(modelData);
	model.draw(*program);

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

	meshes[0]->draw(*programs[0]);

}