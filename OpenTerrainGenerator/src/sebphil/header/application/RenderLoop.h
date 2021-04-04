#pragma once
#include <functional>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "application/Window.h"
#include "application/FrameClock.h"

#include "modelstructure/Model.h"
#include "globjects/UniformBuffer.h"
#include "globjects/Texture.h"
#include "globjects/Framebuffer.h"
#include "globjects/MultisampleTexture.h"
#include "globjects/MultisampleRenderBuffer.h"
#include "modelstructure/ScreenMesh.h"
#include "camera/Camera.h"
#include "scene/Scene.h"

namespace otg {
	class RenderLoop {

	public:
		RenderLoop(Window& window);

		void start(Scene& scene);
		
		void setInputFunc(const std::function<void(GLFWwindow* window)>& function);

	private:
		bool active;

		Window* window;
		FrameClock clock;

		void setUpCam(Camera& cam);
		void addCubeModel(std::vector<std::shared_ptr<Model>>& models);
		void setUpCallbacks(Scene& scene);

		void tick(Scene& scene);
		void update(Scene& scene);
		void draw(Scene& scene);

		void processInput();

		static void standardInputProcessing(GLFWwindow* window);

	};
}