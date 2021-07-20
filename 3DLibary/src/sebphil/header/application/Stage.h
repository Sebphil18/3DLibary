#pragma once
#include "application/Window.h"
#include "application/Scene.h"
#include "application/VirtualScreen.h"
#include "application/DebugMessenger.h"
#include "application/FrameClock.h"
#include "gui/ImGuiEnvironment.h"
#include "exceptions/ApplicationException.h"
#include <iostream>
#include <functional>

// TODO: prototype
namespace glib {

	class Stage : public glib::Window {

	public:

		typedef std::function<void(glib::Scene& scene, const glib::FrameClock& clock)> RenderFunction;

		Stage(const std::string& title = "3DLibary", int width = 1200, int height = 800);
		Stage(const std::shared_ptr<Scene>& scene, const std::string& title = "3DLibary", int width = 1200, int height = 800);

		void setScene(const std::shared_ptr<Scene>& scene);

		void startRenderLoop();

		std::shared_ptr<Scene>& getScene();

		void setOnUpdate(const RenderFunction& function);
		void setOnDraw(const RenderFunction& function);
		void setOnGuiDraw(const RenderFunction& function);

	private:
		std::shared_ptr<Scene> scene;
		DebugMessenger messenger;
		VirtualScreen screen;
		FrameClock clock;
		ImGuiEnvironment imGuiEnv;

		std::function<void(Scene& scene, const FrameClock& clock)> updateFunc;
		std::function<void(Scene& scene, const FrameClock& clock)> drawFunc;
		std::function<void(Scene& scene, const FrameClock& clock)> guiDrawFunc;

		void setCallbacks();
		
		void launchRenderLoop();
		void update();
		void draw();

		static void standartUpdateFunc(Scene& scene, const FrameClock& clock);
		static void standartDrawFunc(Scene& scene, const FrameClock& clock);
		static void standartGuiDrawFunc(Scene& scene, const FrameClock& clock);

	};

}