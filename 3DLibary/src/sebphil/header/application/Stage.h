#pragma once
#include "application/Window.h"
#include "application/Scene.h"
#include "application/VirtualScreen.h"
#include "application/DebugMessenger.h"
#include "application/FrameClock.h"
#include "exceptions/ApplicationException.h"
#include <iostream>
#include <functional>

// TODO: prototype
namespace glib {

	class Stage : public glib::Window {

	public:

		typedef std::function<void(glib::Scene& scene, const glib::FrameClock& clock)> RenderFunction;

		Stage(const std::string& title = "3DLibary", int width = 1200, int height = 800);
		Stage(const std::shared_ptr<glib::Scene>& scene, const std::string& title = "3DLibary", int width = 1200, int height = 800);

		void setScene(const std::shared_ptr<glib::Scene>& scene);

		void startRenderLoop();

		std::shared_ptr<glib::Scene>& getScene();

		void setOnUpdate(const RenderFunction& function);
		void setOnDraw(const RenderFunction& function);

	private:
		std::shared_ptr<glib::Scene> scene;
		glib::DebugMessenger messenger;
		glib::VirtualScreen screen;
		glib::FrameClock clock;

		std::function<void(glib::Scene& scene, const glib::FrameClock& clock)> updateFunc;
		std::function<void(glib::Scene& scene, const glib::FrameClock& clock)> drawFunc;

		void setCallbacks();
		
		void launchRenderLoop();
		void update();
		void draw();

		static void standartUpdateFunc(glib::Scene& scene, const glib::FrameClock& clock);
		static void standartDrawFunc(glib::Scene& scene, const glib::FrameClock& clock);

	};

}