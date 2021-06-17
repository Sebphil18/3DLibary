#pragma once
#include "application/Window.h"
#include "imgui/imgui.h"

namespace otg {

	class ImGuiEnvironment {

	public:
		ImGuiEnvironment(const Window& window) noexcept;
		ImGuiEnvironment(const ImGuiEnvironment& other) = delete;
		~ImGuiEnvironment() noexcept;

		ImGuiEnvironment& operator=(const ImGuiEnvironment& other) = delete;

		void newFrame();
		void endFrame();

	private:
		ImGuiIO* io;

		void setIo();
		void setStyle();
		void initImplementations(const Window& window);

		void renderAdditionalWindows();

	};

}