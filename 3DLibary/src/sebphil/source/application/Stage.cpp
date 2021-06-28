#include "application/Stage.h"

namespace glib {

	Stage::Stage(const std::string& title, int width, int height) :
		glib::Window(title, width, height),
		screen(width, height) {

		setCallbacks();
		updateFunc = standartUpdateFunc;
		drawFunc = standartDrawFunc;
	}

	Stage::Stage(const std::shared_ptr<glib::Scene>& scene, const std::string& title, int width, int height) :
		glib::Window(title, width, height),
		screen(width, height),
		scene(scene) {

		setCallbacks();
		updateFunc = standartUpdateFunc;
		drawFunc = standartDrawFunc;
	}

	void Stage::setCallbacks() {

		setSizeCallback([&](GLFWwindow* window, int width, int height) {
			screen.setSize(width, height);
			scene->setSize(width, height);
			glViewport(0, 0, width, height);
			});
	}

	void Stage::setScene(const std::shared_ptr<glib::Scene>& scene) {

		if (scene == nullptr)
			throw ApplicationException("Scene cannot be null!");

		this->scene = scene;
	}

	void Stage::startRenderLoop() {
		try {
			launchRenderLoop();
		} catch (const glib::ApplicationException& exception) {
			std::cout << exception.what() << "\n";
		}
	}


	void Stage::launchRenderLoop() {

		if (scene == nullptr)
			throw glib::ApplicationException("Stage has no scene to render! \n");

		while (!shouldClose()) {

			update();
			draw();

			glfwSwapBuffers(getGlfwWindow());
			glfwPollEvents();
		}
	}

	void Stage::update() {
		clock.tick();
		scene->update();
		updateFunc(*scene, clock);
	}
	
	void Stage::draw() {

		screen.beginFrame();

		scene->draw();
		drawFunc(*scene, clock);

		screen.endFrame(*scene->programs["screen"]);
		screen.draw();
	}

	std::shared_ptr<glib::Scene>& Stage::getScene() {

		try {
			if (scene == nullptr)
				throw glib::ApplicationException("Stage has no scene to render! \n");
		} catch (const glib::ApplicationException& exception) {
			std::cout << exception.what() << "\n";
		}

		return scene;
	};

	void Stage::setOnUpdate(const RenderFunction& function) {
		updateFunc = function;
	}

	void Stage::setOnDraw(const RenderFunction& function) {
		drawFunc = function;
	}

	void Stage::standartUpdateFunc(Scene& scene, const glib::FrameClock& clock) {}
	void Stage::standartDrawFunc(glib::Scene& scene, const glib::FrameClock& clock) {}

}