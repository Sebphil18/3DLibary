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
#include "scene/Scene.h"

#include "stb/stbimage.h"

static std::vector<std::shared_ptr<otg::Model>> models;
static std::unordered_map<std::string, std::shared_ptr<otg::ShaderProgram>> programs;
static void launchApp();

// TODO: add CubeMapArray support for meshes
// TODO: add SkyBox class
// TODO: refactor shaders

int main() {
	launchApp();
}

static void launchApp() {

	stbi_set_flip_vertically_on_load(true);

	otg::Application app;

	otg::Window window("OpenTerrainGenerator");
	int width = window.getWidth();
	int height = window.getHeight();

	otg::DebugMessenger debugMessenger;

	GLFWwindow* winHandle = window.getGlfwWindow();
	otg::FrameClock clock;

	// Model
	otg::ModelLoader loader("rec/shapes/sphere/sphereobj.obj");
	std::shared_ptr<otg::Model> model = std::make_shared<otg::Model>(loader.getData());
	model->meshes[0].setMaterial({ 0, 0, 1, glm::vec3(0), glm::vec3(0), glm::vec3(0) });

	std::shared_ptr<otg::TextureImage> albedoMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/tiles/Albedo.png", otg::TextureType::Albedo);
	model->addTexture(albedoMap, 0);

	std::shared_ptr<otg::TextureImage> roughnessMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/tiles/Roughness.png", otg::TextureType::Roughness);
	model->addTexture(roughnessMap, 0);

	std::shared_ptr<otg::TextureImage> metallicMap = std::make_shared<otg::TextureImage>(
		"C:/Users/User/source/repos/OpenTerrainGenerator/OpenTerrainGenerator/rec/textures/tiles/Metallic.png", otg::TextureType::Metallic);
	model->addTexture(metallicMap, 0);

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
	otg::Camera cam;
	cam.setWidth(window.getWidth());
	cam.setHeight(window.getHeight());
	cam.setPosition(glm::vec3(1.5, 0, -1));

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

	// CubeMap - convert equirectengular texture to cubemap
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
	skyBoxProbe.bindToUnit(6);

	// SpecularIBL
	otg::SkyBoxReflectionProbe reflectionProbe(envMap, programs["prefilter"], programs["brdfIntegration"]);
	reflectionProbe.bindPrefilterToUnit(7);
	reflectionProbe.bindLookUpTexToUnit(8);

	programs["main"]->setUniform("prefilterMap", 7);
	programs["main"]->setUniform("brdf", 8);

	// SkyBox
	otg::Mesh skyBox(cubeVertices, cubeIndices);

	// RenderLoop
	while (!window.shouldClose()) {

		clock.tick();

		model->setRotation(glm::vec3(std::sin(0.002 * clock.getCurrentFrame()) * 2 * 3.14, 
			std::cos(0.002 * clock.getCurrentFrame()) * 2 * 3.14,
			0));

		model2->setRotation(glm::vec3(0, 0.01 * clock.getCurrentFrame(), 0));

		programs["main"]->use();
		programs["main"]->setUniformVec("viewPos", cam.getPosition());
		programs["main"]->setUniformVec("lightPos", glm::vec3(-1, 1, -1.5));
		programs["main"]->setUniform("envMap", 6);

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

		screenFbo.clear();
		multiFbo.copyColorTo(width, height, screenFbo);

		screen.draw(*programs["screen"]);

		glfwSwapBuffers(winHandle);
		glfwPollEvents();
	}

}