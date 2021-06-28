#pragma once
#include "application/Window.h"
#include "application/Scene.h"
#include "application/VirtualScreen.h"
#include "application/DebugMessenger.h"
#include "application/FrameClock.h"
#include "exceptions/ApplicationException.h"
#include <iostream>

// TODO: prototype
namespace glib {

	class Stage : public glib::Window {

	public:
		Stage(const std::string& title = "3DLibary", int width = 1200, int height = 800);
		Stage(const std::shared_ptr<glib::Scene>& scene, const std::string& title = "3DLibary", int width = 1200, int height = 800);

		void setScene(const std::shared_ptr<glib::Scene>& scene);

		void startRenderLoop();

		std::shared_ptr<glib::Scene>& getScene();

	private:
		std::shared_ptr<glib::Scene> scene;
		glib::DebugMessenger messenger;
		glib::VirtualScreen screen;
		glib::FrameClock clock;

		void setCallbacks();

		void launchRenderLoop();

	};

}