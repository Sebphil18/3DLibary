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
#include "application/FrameClock.h"
#include "application/Window.h"
#include "application/VirtualScreen.h"
#include "application/Mouse.h"

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
#include "modelstructure/LoadRegister.h"

#include "lighting/SkyBox.h"
#include "lighting/HDRTextureCube.h"
#include "lighting/SkyBoxLightProbe.h"
#include "lighting/SkyBoxReflectionProbe.h"

#include "camera/OrbitCamera.h"
#include "camera/FpsCamera.h"

#include "terrain/Heightmap.h"
#include "terrain/TerrainModelLoader.h"

#include "stb/stbimage.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "gui/ImGuiEnvironment.h"
#include "debugflags/DebugFlags.h"

static std::vector<std::shared_ptr<otg::Model>> models;
static std::unordered_map<std::string, std::shared_ptr<otg::ShaderProgram>> programs;

static otg::FrameClock frameClock;

static void launchApp();

static void addModels();
static void addPreviewModel();

static void addPrograms();

static otg::CubeMapArray getEnvMap();

static void clearScreen();
static void update(const otg::Camera& cam, otg::UniformBuffer& matrices);
static void draw(otg::VirtualScreen& virtualScreen, const otg::Camera& cam, otg::UniformBuffer& matrices, otg::SkyBox& skybox);
static void drawModels(otg::UniformBuffer& matrices);
static void drawModel(std::shared_ptr<otg::Model>& model, otg::UniformBuffer& matrices);

static void processKeyInput(const otg::Window& window, otg::Camera& camera, float frameTime);
static void processMouseInput(otg::Mouse& mouse, otg::Camera& camera);

// TOOD: refactor this file
// TODO: add ImGuiLayer
// TODO: Viewport class

int main() {
	launchApp();
}

static void launchApp() {

	stbi_set_flip_vertically_on_load(true);

	otg::Application app;

	otg::Window window1("3DLibary");
	int initialWidth = window1.getWidth();
	int initialHeight = window1.getHeight();

	// Callbacks
	window1.setKeyCallback([&](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(window, true);
		});

#if SEB_DEBUG_MODE == 1
	otg::DebugMessenger debugMessenger;
#endif

	otg::VirtualScreen virtualScreen(initialWidth, initialHeight);
	otg::FpsCamera cam(glm::ivec2(initialWidth, initialHeight));
	otg::Mouse mouse(window1);

	// UniformBuffer
	otg::UniformBuffer matrices;
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.addElement({ otg::UniformType::Matrix4 });
	matrices.bindTo(0);

	// Models & Programs
	addModels();
	addPrograms();

	// IBL
	otg::CubeMapArray envMap = getEnvMap();
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
	otg::SkyBox skyBox(12);

	// ImGUI
	otg::ImGuiEnvironment imGuiEnv(window1);

	ImVec2 winSize = {1, 1};
	bool viewPortOpen = true;

	otg::Material testMat;
	float albedoColor[3] = { 1, 0.2, 0};
	float roughness = 0.5;
	float metallic = 0;
	float occlusion = 1;

	testMat.setAlbedo(glm::vec3(albedoColor[0], albedoColor[1], albedoColor[2]));
	testMat.setRoughness(roughness);
	testMat.setMetallic(metallic);
	testMat.setOcclusion(occlusion);

	models[0]->meshes[0].setMaterial(testMat);

	// needs: clock, skyBoxMesh, virtualscreen, window1, ubo, models, programs
	// RenderLoop
	while (!window1.shouldClose()) {

		update(cam, matrices);
		draw(virtualScreen, cam, matrices, skyBox);

		// new ImGui frame
		imGuiEnv.newFrame();

		ImGui::DockSpaceOverViewport();

		// main Viewport
		ImGui::Begin("Viewport", &viewPortOpen, ImGuiWindowFlags_NoScrollWithMouse);

		ImVec2 newWinSize = ImGui::GetWindowSize();
		if (newWinSize.x != winSize.x || newWinSize.y != winSize.y) {
			winSize = newWinSize;
			glViewport(0, 0, winSize.x, winSize.y);
			cam.setSize(winSize.x, winSize.y);
			virtualScreen.setSize(winSize.x, winSize.y);
		}

		if (ImGui::IsWindowFocused()) {
			processKeyInput(window1, cam, frameClock.getFrameTime());
			processMouseInput(mouse, cam);
		} else {
			mouse.resetPosition();
			mouse.resetScrollDelta();
		}

		std::uint32_t screenHandle = virtualScreen.getScreenTexHandle();
		ImTextureID texID = (void*)screenHandle;
		ImGui::Image(texID, winSize, { 0, 1 }, { 1, 0 });

		ImGui::End();

		ImGui::Begin("Material");

		if (ImGui::ColorEdit3("albedo", albedoColor)) {
			testMat.setAlbedo(glm::vec3(albedoColor[0], albedoColor[1], albedoColor[2]));
			models[0]->meshes[0].setMaterial(testMat);
		}

		if (ImGui::SliderFloat("roughness", &roughness, 0, 1)) {
			testMat.setRoughness(roughness);
			models[0]->meshes[0].setMaterial(testMat);
		}

		if (ImGui::SliderFloat("metallic", &metallic, 0, 1)) {
			testMat.setMetallic(metallic);
			models[0]->meshes[0].setMaterial(testMat);
		}

		if (ImGui::SliderFloat("occlusion", &occlusion, 0, 1)) {
			testMat.setOcclusion(occlusion);
			models[0]->meshes[0].setMaterial(testMat);
		}

		ImGui::End();

		imGuiEnv.endFrame();

		glfwSwapBuffers(window1.getGlfwWindow());
		glfwPollEvents();
	}
}

void addModels() {
	addPreviewModel();
}

void addPreviewModel() {

	otg::ModelLoader sphereLoader("rec/shapes/sphere/Sphere.obj");
	std::shared_ptr<otg::Model> sphere = std::make_shared<otg::Model>(sphereLoader.getData());
	sphere->setPosition(glm::vec3(3, 0, 0));
	models.push_back(sphere);

	// orange
	otg::Material orange;
	orange.setOcclusion(0);
	std::shared_ptr<otg::TextureImage> albedoMap = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Orange_Base_Color.jpg", otg::TextureType::Albedo);
	orange.addTexture(albedoMap);

	std::shared_ptr<otg::TextureImage> normalMap = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Orange_Normal_OpenGL.jpg", otg::TextureType::Normal);
	orange.addTexture(normalMap);

	std::shared_ptr<otg::TextureImage> roughnessMap = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Orange_Roughness.jpg", otg::TextureType::Roughness);
	orange.addTexture(roughnessMap);

	std::shared_ptr<otg::TextureImage> metallicMap = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Orange_Metallic.jpg", otg::TextureType::Roughness);
	orange.addTexture(metallicMap);

	std::shared_ptr<otg::TextureImage> aoMap = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Orange_Mixed_AO.jpg", otg::TextureType::Occlusion);
	orange.addTexture(aoMap);

	// black
	otg::Material black;
	black.setOcclusion(0);
	std::shared_ptr<otg::TextureImage> albedoMapBlack = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Black_Base_Color.jpg", otg::TextureType::Albedo);
	black.addTexture(albedoMapBlack);

	std::shared_ptr<otg::TextureImage> normalMapBlack = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Black_Normal_OpenGL.jpg", otg::TextureType::Normal);
	black.addTexture(normalMapBlack);

	std::shared_ptr<otg::TextureImage> roughnessMapBlack = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Black_Roughness.jpg", otg::TextureType::Roughness);
	black.addTexture(roughnessMapBlack);

	std::shared_ptr<otg::TextureImage> metallicMapBlack = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Black_Metallic.jpg", otg::TextureType::Roughness);
	black.addTexture(metallicMapBlack);

	std::shared_ptr<otg::TextureImage> aoMapBlack = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Black_Mixed_AO.jpg", otg::TextureType::Occlusion);
	black.addTexture(aoMapBlack);

	//white
	otg::Material white;
	std::shared_ptr<otg::TextureImage> albedoMapWhite = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/White_Base_Color.jpg", otg::TextureType::Albedo);
	white.addTexture(albedoMapWhite);

	std::shared_ptr<otg::TextureImage> normalMapWhite = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/White_Normal_OpenGL.jpg", otg::TextureType::Normal);
	white.addTexture(normalMapWhite);

	std::shared_ptr<otg::TextureImage> roughnessMapWhite = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/White_Roughness.jpg", otg::TextureType::Roughness);
	white.addTexture(roughnessMapWhite);

	std::shared_ptr<otg::TextureImage> metallicMapWhite = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/White_Metallic.jpg", otg::TextureType::Roughness);
	white.addTexture(metallicMapWhite);

	//black2;
	otg::Material black2;
	black2.setAlbedo(glm::vec3(0, 0, 0));
	black2.setRoughness(0.11);
	black2.setMetallic(0.5);

	std::shared_ptr<otg::TextureImage> face = std::make_shared<otg::TextureImage>(
		"rec/shapes/bot/Textures/Faces2.jpg", otg::TextureType::Albedo);
	black2.addTexture(face);

	otg::ModelLoader loader("rec/shapes/bot/BotObj.obj");
	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>(loader.getData());

	model->meshes[0].setMaterial(orange);
	model->meshes[1].setMaterial(black);
	model->meshes[2].setMaterial(white);
	model->meshes[3].setMaterial(black2);
	models.push_back(model);

	otg::ModelLoader planeLoader("rec/shapes/plane/Plane.obj");
	std::shared_ptr<otg::Model> plane = std::make_shared<otg::Model>(planeLoader.getData());
	plane->setScale(glm::vec3(10, 10, 10));
	plane->setPosition(glm::vec3(0, -5, 0));
	models.push_back(plane);

}

void addPrograms() {

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

	programs["equirectConv"] = std::make_shared<otg::ShaderProgram>(
		"src/sebphil/shader/vertex/EquirectangularToCube.glsl",
		"src/sebphil/shader/fragment/EquirectangularToCube.glsl"
		);

	programs["main"]->setUniformBlockBinding("Matrices", 0);
}

otg::CubeMapArray getEnvMap() {

	std::shared_ptr<otg::HDRTexture> equiRectTexture = std::make_shared<otg::HDRTexture>("rec/textures/hdr/outdoor/Helipad.hdr");

	otg::CubeMapArray envMap({ 512, 512 });
	envMap.fromEquirectengular(equiRectTexture, *programs["equirectConv"]);

	return envMap;
}

void update(const otg::Camera& cam, otg::UniformBuffer& matrices) {

	frameClock.tick();

	programs["main"]->use();
	programs["main"]->setUniformVec("viewPos", cam.getPosition());
	programs["main"]->setUniformVec("lightPos", glm::vec3(-1, 1, -1.5) * 10.0f);

	glm::mat4 view = cam.getViewMatrix();
	glm::mat4 projection = cam.getProjectionMatrix();

	matrices.setElementData(2, glm::value_ptr(view));
	matrices.setElementData(3, glm::value_ptr(projection));
}

void draw(otg::VirtualScreen& virtualScreen, const otg::Camera& cam, otg::UniformBuffer& matrices, otg::SkyBox& skybox) {

	clearScreen();
	virtualScreen.beginFrame();

	drawModels(matrices);
	skybox.draw(cam, *programs["skyBox"]);

	virtualScreen.endFrame(*programs["screen"]);
}

void clearScreen() {

	glClearColor(0.1, 0.1, 0.1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);
}

void drawModels(otg::UniformBuffer& matrices) {

	for (std::shared_ptr<otg::Model>& model : models)
		drawModel(model, matrices);
}

void drawModel(std::shared_ptr<otg::Model>& model, otg::UniformBuffer& matrices) {

	glm::mat4 world = model->getWorldMatrix();
	glm::mat4 normal = model->getNormalMatrix();

	matrices.setElementData(0, glm::value_ptr(world));
	matrices.setElementData(1, glm::value_ptr(normal));

	model->draw(*programs["main"]);
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

void processMouseInput(otg::Mouse& mouse, otg::Camera& camera) {

	mouse.move();
	if (mouse.isPrimaryPressed())
		camera.move(mouse.getPosDelta());

	std::int32_t scrollOffsetY = mouse.getScrollOffsetY();
	if (scrollOffsetY != 0) {
		camera.moveForward(scrollOffsetY);
		mouse.resetScrollDelta();
	}
}