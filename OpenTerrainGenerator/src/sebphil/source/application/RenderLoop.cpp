#include "application/RenderLoop.h"
#include <iostream>
#include <math.h>
#include "glm/gtc/type_ptr.hpp"
#include "modelstructure/CubeData.h"

namespace otg {
	
	RenderLoop::RenderLoop(Window& window) : 
		window(&window), active(false)
	{
	}

	// TODO: ForwardPipeline class
	void RenderLoop::start(Scene& scene) {

		active = true;
		window->focus();

		setUpCallbacks(scene);
		setUpCam(scene.cam);
		addCubeModel(scene.models);

		while (!window->shouldClose())
			tick(scene);
	}

	void RenderLoop::setUpCam(Camera& cam) {

		cam.setPosition(glm::vec3(0, 0, -2));
		cam.setWidth(window->getWidth());
		cam.setHeight(window->getHeight());
	}

	void RenderLoop::addCubeModel(std::vector<std::shared_ptr<Model>>& models) {

		std::shared_ptr<Model> cube = std::make_shared<Model>();
		std::shared_ptr<TextureImage> texture = std::make_shared<TextureImage>("rec/textures/testtexture/HdrTest.png", TextureType::Albedo);
		cube->meshes.emplace_back(unitCubeVertices, cubeIndices);
		cube->setRotation(glm::vec3(1, 1, 1));

		models.push_back(cube);
	}

	void RenderLoop::setUpCallbacks(Scene& scene) {

		// TODO: too long; move into ForwardPipeline class (when created)
		window->setSizeCallback([&](GLFWwindow* window, int width, int height) {

			glViewport(0, 0, width, height);
			scene.cam.setWidth(width);
			scene.cam.setHeight(height);

			});

		window->setKeyCallback([&](GLFWwindow* window, int key, int scancode, int action, int mods) {

			switch (key) {

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, true);
				break;
			}

			});

	}

	void RenderLoop::tick(Scene& scene) {

		update(scene);
		draw(scene);

		glfwSwapBuffers(window->getGlfwWindow());
		glfwPollEvents();

		processInput();
	}

	void RenderLoop::update(Scene& scene) {

		clock.tick();

		//unsigned long frame = clock.getCurrentFrame();
		//scene.models[0]->setRotation(glm::vec3(sin(frame * 0.02), cos(frame * 0.02), frame * 0.01));

	}

	void RenderLoop::draw(Scene& scene) {

		glClearColor(0.01, 0.01, 0.01, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	}

	void RenderLoop::processInput() {

		GLFWwindow* glfwHandle = window->getGlfwWindow();

		if (glfwGetKey(glfwHandle, GLFW_KEY_W)) {
			//std::cout << "w pressed \n";
		}
	}

}