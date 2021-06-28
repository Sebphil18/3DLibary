#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace glib {

	// TODO: move callbacks into own class: CallbackRegister
	typedef std::function<void(GLFWwindow* window, int width, int height)> WindowSizeCallback;
	typedef std::function<void(GLFWwindow* window, int  key, int scancode, int action, int mods)> WindowInputCallback;
	typedef std::function<void(GLFWwindow* window, double xpos, double ypos)> WindowCoursorPosCallback;
	typedef std::function<void(GLFWwindow* window, double xoffset, double yoffset)> WindowScrollCallback;

	class Window {

	public:
		Window(const std::string& title = "3DLibary", int width = 1200, int height = 800);
		~Window();

		void focus() const noexcept;

		int getWidth() const noexcept;
		int getHeight() const noexcept;
		GLFWwindow* const getGlfwWindow() const noexcept;

		void setTitle(const std::string& title);

		void setSizeCallback(const WindowSizeCallback& callback);
		void setKeyCallback(const WindowInputCallback& callback);
		void setCoursorPosCallback(const WindowCoursorPosCallback& callback);
		void setScrollCallback(const WindowScrollCallback& callback);

		bool shouldClose() const;

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

		void setUpOpenGL();

		void removeCallbacks();

		static std::unordered_map<GLFWwindow*, WindowSizeCallback> sizeCallbacks;
		static void framebufferSizeCallback(GLFWwindow* window, int width, int height) noexcept;

		static std::unordered_map<GLFWwindow*, WindowInputCallback> inputCallbacks;
		static void keyCallback(GLFWwindow* window, int  key, int scancode, int action, int mods);

		static std::unordered_map<GLFWwindow*, WindowCoursorPosCallback> coursorPosCallbacks;
		static void coursorPosCallback(GLFWwindow* window, double xpos, double ypos);

		static std::unordered_map<GLFWwindow*, WindowScrollCallback> scrollCallbacks;
		static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	};
}