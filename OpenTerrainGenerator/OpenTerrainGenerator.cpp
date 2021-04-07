#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

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

#include "io/Image.h"

#include "modelstructure/CubeData.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/ScreenMesh.h"
#include "modelstructure/Model.h"
#include "modelstructure/ModelLoader.h"
#include "modelstructure/TextureRegister.h"
#include "modelstructure/ModelDataConverter.h"

#include "camera/Camera.h"
#include "scene/Scene.h"

#include "stb/stbimage.h"

static std::vector<std::shared_ptr<otg::Model>> models;
static std::unordered_map<std::string, std::shared_ptr<otg::ShaderProgram>> programs;
static void launchApp();

/* TODO: DefferedModelData
*	- global (static) TextureRegister so no texture is loaded twice
*/

int main() {
	launchApp();
}

static void launchApp() {

	// TODO: move into image loading??
	stbi_set_flip_vertically_on_load(true);

	otg::Application app;

	otg::Window window("OpenTerrainGenerator");
	int width = window.getWidth();
	int height = window.getHeight();

	otg::DebugMessenger debugMessenger;

	GLFWwindow* winHandle = window.getGlfwWindow();
	otg::FrameClock clock;

	// Cube
	otg::ModelLoader loader("rec/shapes/sphere/SphereCopper.fbx");
	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>(loader.getData());
	model->meshes[0].setMaterial({ 0, 0, glm::vec3(0), glm::vec3(0), glm::vec3(0) });

	std::shared_ptr<otg::TextureImage> roughnessMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/oldcopper/OldCopperRough.png", otg::TextureType::Roughness);
	model->addTexture(roughnessMap, 0);

	std::shared_ptr<otg::TextureImage> metallicMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/oldcopper/OldCopperMetallic.png", otg::TextureType::Metallic);
	model->addTexture(metallicMap, 0);

	models.push_back(model);

	// ScreenMesh
	otg::ScreenMesh screen;

	// ShaderProgram
	programs["main"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/VertexStandard.glsl",
		"src/sebphil/shader/fragment/FragmentStandard.glsl"
		);

	programs["screen"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/VertexScreen.glsl",
		"src/sebphil/shader/fragment/FragmentScreen.glsl"
		);

	// UniformBuffer
	otg::UniformBuffer matrices;
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.bindTo(*programs["main"], "Matrices", 0);
	
	// Camera
	otg::Camera cam;
	cam.setWidth(window.getWidth());
	cam.setHeight(window.getHeight());
	cam.setPosition(glm::vec3(0, 0, -3));

	// Framebuffer
	otg::Framebuffer multiFbo;
	otg::MultisampleTexture multiTex(width, height, 6);
	otg::MultisampleRenderBuffer multiRbo(width, height, 6);

	multiFbo.attachTexture(multiTex);
	multiFbo.attachRenderBuffer(multiRbo);
	multiFbo.validate();

	otg::Framebuffer screenFbo;
	std::shared_ptr<otg::Texture> screenTex = std::make_shared<otg::Texture>(width, height, otg::TextureType::ColorAttachment);
	otg::RenderBuffer screenRbo(width, height, otg::TextureType::DepthStencilAttachment);

	screenFbo.attachTexture(*screenTex);
	screenFbo.attachRenderBuffer(screenRbo);
	screenFbo.validate();

	screen.addTexture(screenTex);

	// Callbacks
	window.setSizeCallback([&](GLFWwindow* window, int width, int height) {

		glViewport(0, 0, width, height);
		cam.setSize(width, height);

		});

	window.setKeyCallback([&](GLFWwindow* window, int key, int scancode, int action, int mods) {

		switch (key) {

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
		}

		});

	// RenderLoop
	while (!window.shouldClose()) {

		clock.tick();

		model->setRotation(glm::vec3(0.01 * clock.getCurrentFrame(), 0, 0));
		programs["main"]->use();
		programs["main"]->setUniformVec("viewPos", cam.getPosition());
		programs["main"]->setUniformVec("lightPos", glm::vec3(0.5, -1, -1.5));

		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_STENCIL_BUFFER_BIT);
		
		glm::mat4 view = cam.getViewMatrix();
		glm::mat4 projection = cam.getProjectionMatrix();

		matrices.setElementData(2, glm::value_ptr(view));
		matrices.setElementData(3, glm::value_ptr(projection));

		for (std::shared_ptr<otg::Model>& model : models) {

			glm::mat4 world = model->getWorldMatrix();
			glm::mat4 normal = model->getNormalMatrix();

			matrices.setElementData(0, glm::value_ptr(world));
			matrices.setElementData(1, glm::value_ptr(normal));

			multiFbo.clear();
			multiFbo.bind();
			model->draw(*programs["main"]);
			multiFbo.unbind();

			screenFbo.clear();
			multiFbo.copyColorTo(width, height, screenFbo);

			screen.draw(*programs["screen"]);
		}

		glfwSwapBuffers(winHandle);
		glfwPollEvents();
	}

}