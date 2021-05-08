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
#include "globjects/HDRTexture.h"
#include "globjects/CubeMapArray.h"

#include "io/Image.h"

#include "modelstructure/CubeData.h"
#include "modelstructure/Mesh.h"
#include "modelstructure/ScreenMesh.h"
#include "modelstructure/Model.h"
#include "modelstructure/ModelLoader.h"
#include "modelstructure/TextureRegister.h"
#include "modelstructure/ModelDataConverter.h"

#include "lighting/HDRTextureCube.h"
#include "lighting/SkyBoxLightProbe.h"
#include "lighting/SkyBoxReflectionProbe.h"

#include "camera/Camera.h"
#include "camera/OrbitCamera.h"
#include "camera/FpsCamera.h"

#include "stb/stbimage.h"

static std::vector<std::shared_ptr<otg::Model>> models;
static std::unordered_map<std::string, std::shared_ptr<otg::ShaderProgram>> programs;
static void launchApp();

// TODO: add SkyBox class
// TODO: refactor shaders

int main() {
	launchApp();
}

static void launchApp() {

	stbi_set_flip_vertically_on_load(true);

	otg::Application app;

	otg::Window window1("OpenTerrainGenerator");
	int initialWidth = window1.getWidth();
	int initialHeight = window1.getHeight();

	otg::DebugMessenger debugMessenger;

	GLFWwindow* winHandle = window1.getGlfwWindow();
	otg::FrameClock clock;

	// Model
	otg::ModelLoader loader("rec/shapes/sphere/sphereobj.obj");
	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>(loader.getData());
	model->meshes[0].setMaterial({ 0, 0, 1, glm::vec3(0), glm::vec3(0), glm::vec3(0) });

	std::shared_ptr<otg::TextureImage> albedoMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Albedo.png", otg::TextureType::Albedo);
	model->addTexture(albedoMap, 0);

	std::shared_ptr<otg::TextureImage> roughnessMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Roughness.png", otg::TextureType::Roughness);
	model->addTexture(roughnessMap, 0);

	std::shared_ptr<otg::TextureImage> metallicMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Metallic.png", otg::TextureType::Metallic);
	model->addTexture(metallicMap, 0);

	std::shared_ptr<otg::TextureImage> normalMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Normal.png", otg::TextureType::Normal
		);
	model->addTexture(normalMap, 0);

	/*std::shared_ptr<otg::TextureImage> occMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/ground/Occlusion.png", otg::TextureType::Occlusion);
	model->addTexture(occMap, 0);*/

	//model->setPosition(glm::vec3(4, 0, 0));

	models.push_back(model);

	otg::ModelLoader loader2("rec/shapes/glados/Glados.obj");
	std::shared_ptr<otg::Model> model2 = std::make_shared<otg::Model>(loader2.getData());
	model2->setPosition(glm::vec3(-4, 0, 2));
	model2->setRotation(glm::vec3(0, 3.14 / 2, 0));
	//models.push_back(model2);

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

	otg::ShaderProgram skyBoxProgram(
		"src/sebphil/shader/vertex/SkyBox.glsl",
		"src/sebphil/shader/fragment/SkyBox.glsl");

	// UniformBuffer
	otg::UniformBuffer matrices;
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.bindTo(*programs["main"], "Matrices", 0);
	
	// Camera
	otg::OrbitCamera cam({ initialWidth, initialHeight });

	// Framebuffer
	otg::Framebuffer multiFbo;
	otg::MultisampleTexture multiTex(initialWidth, initialHeight, 6);
	otg::MultisampleRenderBuffer multiRbo(initialWidth, initialHeight, 6);

	multiFbo.attachTexture(multiTex);
	multiFbo.attachRenderBuffer(multiRbo);
	multiFbo.validate();

	otg::Framebuffer screenFbo;
	std::shared_ptr<otg::Texture> screenTex = std::make_shared<otg::Texture>(initialWidth, initialHeight, otg::TextureType::ColorAttachment);
	otg::RenderBuffer screenRbo(initialWidth, initialHeight, otg::TextureType::DepthStencilAttachment);

	screenFbo.attachTexture(*screenTex);
	screenFbo.attachRenderBuffer(screenRbo);
	screenFbo.validate();

	screen.addTexture(screenTex);

	// Callbacks
	window1.setSizeCallback([&](GLFWwindow* window, int width, int height) {

		glViewport(0, 0, width, height);
		cam.setSize(width, height);

		// TODO: resizeFbos
		screenTex->setSize(width, height);
		screenRbo.setSize(width, height);
		multiTex.setSize(width, height);
		multiRbo.setSize(width, height);

		screenFbo.attachTexture(*screenTex);
		screenFbo.attachRenderBuffer(screenRbo);

		multiFbo.attachTexture(multiTex);
		multiFbo.attachRenderBuffer(multiRbo);

		});

	window1.setKeyCallback([&](GLFWwindow* window, int key, int scancode, int action, int mods) {

		switch (key) {

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);

		}

		});

	double scrollOffsetY = 0;

	window1.setScrollCallback([&](GLFWwindow* window, double xoffset, double yoffset) {
		scrollOffsetY = -yoffset;
		});

	otg::ShaderProgram equirectConversionProgram(
		"src/sebphil/shader/vertex/EquirectangularToCube.glsl",
		"src/sebphil/shader/fragment/EquirectangularToCube.glsl");

	programs["convolution"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/IrradianceConvolution.glsl",
		"src/sebphil/shader/fragment/IrradianceConvolution.glsl"
		);

	programs["brdfIntegration"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/BRDFConvolution.glsl",
		"src/sebphil/shader/fragment/BRDFConvolution.glsl"
		);

	programs["prefilter"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/PrefilterConvolution.glsl",
		"src/sebphil/shader/fragment/PrefilterConvolution.glsl"
		);

	std::shared_ptr<otg::HDRTexture> equiRectTexture = std::make_shared<otg::HDRTexture>("rec/textures/hdr/outdoor/Sunrise.hdr");

	otg::CubeMapArray envMap({ 512, 512 });
	envMap.fromEquirectengular(equiRectTexture, equirectConversionProgram);
	envMap.bindToUnit(0);

	// DiffuseIBL
	otg::SkyBoxLightProbe skyBoxProbe(envMap, programs["convolution"]);

	// SpecularIBL
	otg::SkyBoxReflectionProbe reflectionProbe(envMap, programs["prefilter"], programs["brdfIntegration"]);

	skyBoxProbe.bindToUnit(10);
	reflectionProbe.bindPrefilterToUnit(11);
	reflectionProbe.bindLookUpTexToUnit(12);

	programs["main"]->setUniform("envMap", 10);
	programs["main"]->setUniform("prefilterMap", 11);
	programs["main"]->setUniform("brdf", 12);

	// SkyBox
	otg::Mesh skyBox(cubeVertices, cubeIndices);

	// MouseInput
	float mouseDeltaX = 0;
	float mouseDeltaY = 0;

	double lastMouseX = -1;
	double lastMouseY = -1;

	// RenderLoop
	while (!window1.shouldClose()) {

		clock.tick();

		programs["main"]->use();
		programs["main"]->setUniformVec("viewPos", cam.getPosition());
		programs["main"]->setUniformVec("lightPos", glm::vec3(-1, 1, -1.5));

		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glClear(GL_STENCIL_BUFFER_BIT);

		glm::mat4 view = cam.getViewMatrix();
		glm::mat4 projection = cam.getProjectionMatrix();

		matrices.setElementData(2, glm::value_ptr(view));
		matrices.setElementData(3, glm::value_ptr(projection));

		multiFbo.clear();
		multiFbo.bind();
		for (std::shared_ptr<otg::Model>& model : models) {

			glm::mat4 world = model->getWorldMatrix();
			glm::mat4 normal = model->getNormalMatrix();

			matrices.setElementData(0, glm::value_ptr(world));
			matrices.setElementData(1, glm::value_ptr(normal));

			model->draw(*programs["main"]);
		}
		
		// TODO: SkyBox::draw();
		glDepthFunc(GL_LEQUAL);
		skyBoxProgram.setUniformMat("view", cam.getViewMatrix());
		skyBoxProgram.setUniformMat("projection", cam.getProjectionMatrix());
		skyBoxProgram.setUniform("cubeMap", 0);
		skyBox.draw(skyBoxProgram);
		glDepthFunc(GL_LESS);

		multiFbo.unbind();

		// draw to screen-fbo
		screenFbo.clear();
		multiFbo.copyColorTo(window1.getWidth(), window1.getHeight(), screenFbo);

		screen.draw(*programs["screen"]);

		// TODO: INPUT HANDLING
		// mouse input
		double currentMouseX, currentMouseY;
		glfwGetCursorPos(window1.getGlfwWindow(), &currentMouseX, &currentMouseY);

		if (lastMouseX != -1 && lastMouseY != -1) {

			mouseDeltaX = lastMouseX - currentMouseX;
			mouseDeltaY = currentMouseY - lastMouseY;
		}

		lastMouseX = currentMouseX;
		lastMouseY = currentMouseY;

		if (scrollOffsetY != 0) {
			cam.moveForward(scrollOffsetY);
			scrollOffsetY = 0;
		}

		// keyboard input
		if (glfwGetKey(window1.getGlfwWindow(), GLFW_KEY_D) == GLFW_PRESS) {
			cam.moveRight(clock.getFrameTime());
		} 
		if (glfwGetKey(window1.getGlfwWindow(), GLFW_KEY_A) == GLFW_PRESS) {
			cam.moveLeft(clock.getFrameTime());
		} 
		if (glfwGetKey(window1.getGlfwWindow(), GLFW_KEY_W) == GLFW_PRESS) {
			cam.moveUp(clock.getFrameTime());
		} 
		if (glfwGetKey(window1.getGlfwWindow(), GLFW_KEY_S) == GLFW_PRESS) {
			cam.moveDown(clock.getFrameTime());
		}
		if (glfwGetMouseButton(window1.getGlfwWindow(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
			cam.move({ mouseDeltaX, mouseDeltaY }, clock.getFrameTime());
		}

		glfwSwapBuffers(winHandle);
		glfwPollEvents();
	}

}