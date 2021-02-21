#pragma once
#include <string>
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace otg {

	class Window {

	public:
		Window(const std::string& title, int width = 1200, int height = 800);
		~Window();

		void focus() const noexcept;

		int getWidth() const noexcept;
		int getHeight() const noexcept;
		GLFWwindow* const getGlfwWindow() const noexcept;

		void setFramebufferSizeCallback(GLFWframebuffersizefun func);

	private:
		int width, height;
		std::string title;

		GLFWwindow* window;

		void generateWindow();
		void setWindowHints();
		void makeContext();
		void createWindow();
		void loadGlBindings();
		void setWindowCallbacks() noexcept;
		
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;

		void setUpOpenGL();

	};

}