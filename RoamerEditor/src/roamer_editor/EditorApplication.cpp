#include "roamer_editor/EditorApplication.hpp"
#include "roamer_editor/Hierarchy.hpp"
#include "roamer_editor/Inspector.hpp"

namespace qy::cg::editor {

	void EditorApplication::internalInit() {
		Application::internalInit();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 24.0f);
		//imguiconfigflags
		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void EditorApplication::internalRender() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		onGUI();
		ImGui::Render();
	}

	void EditorApplication::internalLateUpdate() {
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void EditorApplication::onGUI() {
		with_Window("Hierarchy") {
			Hierarchy::instance()->onGUI();
		}
		with_Window("Inspector") {
			Inspector::instance()->onGUI();
		}
	}
}