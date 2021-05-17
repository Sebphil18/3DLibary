#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/noise.hpp"
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

#include "terrain/Heightmap.h"
#include "terrain/TerrainModelLoader.h"

#include "stb/stbimage.h"

static std::vector<std::shared_ptr<otg::Model>> models;
static std::unordered_map<std::string, std::shared_ptr<otg::ShaderProgram>> programs;
static void launchApp();

static void clearScreen();
static void drawSkyBox(const otg::Camera& camera, otg::Mesh& skyBoxMesh);

static glm::vec2 lastMousePos(-1, -1);
static glm::ivec2 scrollOffset(0, 0);
static void processInput(const otg::Window& window, otg::Camera& camera, float frameTime);
static void processKeyInput(const otg::Window& window, otg::Camera& camera, float frameTime);
static void processMouseInput(const otg::Window& window, otg::Camera& camera, float frameTime);
static void processScrollInput(otg::Camera& cam);

// TODO: TerrainGenerator (generate Heightmap; float array)
// TODO: TerrainModelGenerator (uses heightmap to generate DeferredModelData)
// TODO: add ImGui

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

	otg::Material material;
	material.setAlbedo(glm::vec3(0, 0, 0));

	std::shared_ptr<otg::TextureImage> albedoMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Albedo.png", otg::TextureType::Albedo);
	material.addTexture(albedoMap);

	std::shared_ptr<otg::TextureImage> roughnessMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Roughness.png", otg::TextureType::Roughness);
	material.addTexture(roughnessMap);

	std::shared_ptr<otg::TextureImage> metallicMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Metallic.png", otg::TextureType::Metallic);
	material.addTexture(metallicMap);

	std::shared_ptr<otg::TextureImage> normalMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/grass/Normal.png", otg::TextureType::Normal
		);
	material.addTexture(normalMap);

	/*std::shared_ptr<otg::TextureImage> occlusionMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/stone/Occlusion.png", otg::TextureType::Occlusion
		);
	material.addTexture(occlusionMap);*/

	otg::ModelLoader loader("rec/shapes/sphere/sphereobj.obj");
	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>(loader.getData());

	model->meshes[0].setMaterial(material);
	models.push_back(model);

	// Terrain
	std::int32_t size = 255;
	otg::Heightmap<float> heightmap({ size, size });
	for (std::uint32_t x = 0; x < size; x++) {
		for (std::uint32_t y = 0; y < size; y++) {

			glm::ivec2 cell = { x, y };

			float value = 2 * (glm::simplex(glm::vec2(x, y) * 0.03f));

			heightmap.setValue(cell, value * 2);
		}
	}

	otg::TerrainModelLoader terrainLoader(heightmap);

	std::shared_ptr<otg::Model> terrainModel = std::make_shared<otg::Model>(terrainLoader.getData(), GL_DYNAMIC_DRAW);

	terrainModel->meshes[0].setMaterial(material);
	terrainModel->setPosition({ -size / 2, -1, -size / 2 });

	models.push_back(terrainModel);

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
	
	programs["skyBox"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/SkyBox.glsl",
		"src/sebphil/shader/fragment/SkyBox.glsl"
		);

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

	// UniformBuffer
	otg::UniformBuffer matrices;
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.bindTo(*programs["main"], "Matrices", 0);
	
	// Camera
	otg::FpsCamera cam({ initialWidth, initialHeight });

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

	window1.setScrollCallback([&](GLFWwindow* window, double xoffset, double yoffset) {
		scrollOffset.y = -yoffset;
		scrollOffset.x = xoffset;
		});

	otg::ShaderProgram equirectConversionProgram(
		"src/sebphil/shader/vertex/EquirectangularToCube.glsl",
		"src/sebphil/shader/fragment/EquirectangularToCube.glsl");

	std::shared_ptr<otg::HDRTexture> equiRectTexture = std::make_shared<otg::HDRTexture>("rec/textures/hdr/outdoor/Sunrise.hdr");

	otg::CubeMapArray envMap({ 512, 512 });
	envMap.fromEquirectengular(equiRectTexture, equirectConversionProgram);

	// IBL
	otg::SkyBoxLightProbe skyBoxProbe(envMap, programs["convolution"]);
	otg::SkyBoxReflectionProbe reflectionProbe(envMap, programs["prefilter"], programs["brdfIntegration"]);

	envMap.bindToUnit(10);
	skyBoxProbe.bindToUnit(11);
	reflectionProbe.bindPrefilterToUnit(12);
	reflectionProbe.bindLookUpTexToUnit(13);

	programs["main"]->setUniform("envMap", 11);
	programs["main"]->setUniform("prefilterMap", 12);
	programs["main"]->setUniform("brdf", 13);

	// SkyBox
	otg::Mesh skyBox(cubeVertices, cubeIndices);
	
	// RenderLoop
	while (!window1.shouldClose()) {

		clock.tick();

		programs["main"]->use();
		programs["main"]->setUniformVec("viewPos", cam.getPosition());
		programs["main"]->setUniformVec("lightPos", glm::vec3(-1, 1, -1.5) * 10.0f);

		clearScreen();

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
		
		drawSkyBox(cam, skyBox);

		multiFbo.unbind();

		// draw to screen-fbo
		screenFbo.clear();
		multiFbo.copyColorTo(window1.getWidth(), window1.getHeight(), screenFbo);

		screen.draw(*programs["screen"]);

		processInput(window1, cam, clock.getFrameTime());

		glfwSwapBuffers(winHandle);
		glfwPollEvents();
	}
}

void clearScreen() {

	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void drawSkyBox(const otg::Camera& camera, otg::Mesh& skyBoxMesh) {

	glDepthFunc(GL_LEQUAL);
	programs["skyBox"]->setUniformMat("view", camera.getViewMatrix());
	programs["skyBox"]->setUniformMat("projection", camera.getProjectionMatrix());
	programs["skyBox"]->setUniform("cubeMap", 12);
	skyBoxMesh.draw(*programs["skyBox"]);
	glDepthFunc(GL_LESS);
}

void processInput(const otg::Window& window, otg::Camera& camera, float frameTime) {
	processKeyInput(window, camera, frameTime);
	processMouseInput(window, camera, frameTime);
	processScrollInput(camera);
}

void processKeyInput(const otg::Window& window, otg::Camera& camera, float frameTime) {

	GLFWwindow* windowPtr = window.getGlfwWindow();

	if (glfwGetKey(windowPtr, GLFW_KEY_D) == GLFW_PRESS)
		camera.moveRight(frameTime);

	if (glfwGetKey(windowPtr, GLFW_KEY_A) == GLFW_PRESS)
		camera.moveLeft(frameTime);

	if (glfwGetKey(windowPtr, GLFW_KEY_W) == GLFW_PRESS)
		camera.moveUp(frameTime);

	if (glfwGetKey(windowPtr, GLFW_KEY_S) == GLFW_PRESS)
		camera.moveDown(frameTime);
}

void processMouseInput(const otg::Window& window, otg::Camera& camera, float frameTime) {

	GLFWwindow* windowPtr = window.getGlfwWindow();

	double currentX, currentY;
	glfwGetCursorPos(windowPtr, &currentX, &currentY);

	glm::vec2 posDelta(0);

	if (lastMousePos.x != -1 && lastMousePos.y != -1) {

		posDelta.x = lastMousePos.x - currentX;
		posDelta.y = currentY - lastMousePos.y;
	}

	lastMousePos = { currentX, currentY };

	if (glfwGetMouseButton(windowPtr, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		camera.move(posDelta, frameTime);
}

void processScrollInput(otg::Camera& cam) {

	if (scrollOffset.y != 0) {
		cam.moveForward(scrollOffset.y);
		scrollOffset.y = 0;
	}
}