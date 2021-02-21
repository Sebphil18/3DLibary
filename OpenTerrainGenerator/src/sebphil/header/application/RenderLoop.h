#pragma once
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application/FrameClock.h"

namespace otg {
	class RenderLoop {

	public:
		RenderLoop(GLFWwindow* window);

		void start();
		void stop();
		
		bool isRunning() const;

		void setUpdateFunc(const std::function<void(FrameClock&)>& function);
		void setDrawFunc(const std::function<void()>& function);
		void setInputFunc(const std::function<void(GLFWwindow* window)>& function);

	private:
		bool active;

		GLFWwindow* window;
		FrameClock clock;

		std::function<void(FrameClock&)> update;
		std::function<void()> draw;
		std::function<void(GLFWwindow*)> processInput;

		void tick();

		static void standardUpdate(FrameClock& clock);
		static void standardDraw();
		static void standardInputProcessing(GLFWwindow* window);

	};
}