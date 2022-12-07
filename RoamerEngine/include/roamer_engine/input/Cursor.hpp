#pragma once
#include <GLFW/glfw3.h>

namespace qy::cg {

	enum class CursorMode {
		Normal = GLFW_CURSOR_NORMAL,
		Hidden = GLFW_CURSOR_HIDDEN,
		Disabled = GLFW_CURSOR_DISABLED,
	};

	class Cursor {

	public:
		static void __init__(GLFWwindow* window) {
			s_window = window;
		}

		static CursorMode getMode() { return s_mode; }
		static void setMode(CursorMode value) {
			s_mode = value;
			glfwSetInputMode(s_window, GLFW_CURSOR, enum_cast(s_mode));	
		}

	private:
		inline static CursorMode s_mode {CursorMode::Normal};
		inline static GLFWwindow* s_window {nullptr};
	};

}