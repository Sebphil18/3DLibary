#include "gui/ImGuiEnvironment.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace otg {

	ImGuiEnvironment::ImGuiEnvironment(const Window& window) noexcept {

		ImGui::CreateContext();

		setIo();
		setStyle();

		initImplementations(window);
	}

	void ImGuiEnvironment::setIo() {

		io = &ImGui::GetIO();
		io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	}

	void ImGuiEnvironment::setStyle() {

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

	void ImGuiEnvironment::initImplementations(const Window& window) {

		ImGui_ImplGlfw_InitForOpenGL(window.getGlfwWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 460 core");
	}

	ImGuiEnvironment::~ImGuiEnvironment() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiEnvironment::newFrame() {

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiEnvironment::endFrame() {

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			renderAdditionalWindows();
	}

	void ImGuiEnvironment::renderAdditionalWindows() {

		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

}