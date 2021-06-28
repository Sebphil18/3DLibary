#include "application/Stage.h"

namespace glib {

	Stage::Stage(const std::string& title, int width, int height) :
		glib::Window(title, width, height),
		screen(width, height) {
		setCallbacks();
	}

	Stage::Stage(const std::shared_ptr<glib::Scene>& scene, const std::string& title, int width, int height) :
		glib::Window(title, width, height),
		screen(width, height),
		scene(scene) {
		setCallbacks();
	}

	void Stage::setCallbacks() {

		setSizeCallback([&](GLFWwindow* window, int width, int height) {
			screen.setSize(width, height);
			scene->setSize(width, height);
			glViewport(0, 0, width, height);
			});
	}

	void Stage::setScene(const std::shared_ptr<glib::Scene>& scene) {
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

			clock.tick();

			scene->update();
			screen.beginFrame();
			scene->draw();
			screen.endFrame(*scene->programs["screen"]);
			screen.draw();

			glfwSwapBuffers(getGlfwWindow());
			glfwPollEvents();
		}
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

}