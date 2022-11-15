#pragma once
#include "KeyCode.hpp"
#include <array>

namespace qy::cg {

	class Input {

	public:
		static bool anyKey() {
			return __keyCount > 0;
		}

		static bool anyKeyDown() {
			return __keyDown;
		}

		static bool getKeyDown(KeyCode key) {
			return __keyStateNow[static_cast<int>(key)] == GLFW_PRESS;
		}

		static bool getKeyUp(KeyCode key) {
			return __keyStateNow[static_cast<int>(key)] == GLFW_RELEASE;
		}

		static bool getKey(KeyCode key) {
			return __keyState[static_cast<int>(key)];
		}

		static glm::vec3 mousePosition() {
			double xpos {0.0}, ypos {0.0};
			glfwGetCursorPos(__window, &xpos, &ypos);
			return {xpos, ypos, 0};
		}

		static glm::vec2 mouseScrollDelta() {
			return __scroll;
		}

		static bool getMouseButtonDown(int button) {
			return glfwGetMouseButton(__window, button) == GLFW_PRESS;
		}

		static bool getMouseButtonUp(int button) {
			return glfwGetMouseButton(__window, button) == GLFW_RELEASE;
		}

		static bool getMouseButton(int button) {
			return __mouseButtonState[button];
		}

	private:
		static void __init__(GLFWwindow* window) {
			__window = window;
			glfwSetKeyCallback(window, __cbKey);
			glfwSetCursorPosCallback(window, __cbCursorPos);
			glfwSetMouseButtonCallback(window, __cbMouseButton);
			glfwSetScrollCallback(window, __cbScroll);
		}

		static void __update__() {
			__scroll = {};
			__keyDown = false;
			std::ranges::fill(__keyStateNow, -1);
		}

		static void __cbKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (key == GLFW_PRESS) {
				__keyCount++;
				__keyDown = true;
			} else if (key == GLFW_RELEASE) {
				__keyCount--;
			}
			if (key >= 0 && key <= GLFW_KEY_LAST) {
				__keyState[key] = action;
				__keyStateNow[key] = action;
			}
		}
		static void __cbCursorPos(GLFWwindow* window, double xpos, double ypos) {

		}
		static void __cbMouseButton(GLFWwindow* window, int button, int action, int mods) {
			__mouseButtonState[button] = action; // 0 = Relase, 1 = Press
		}
		static void __cbScroll(GLFWwindow* window, double xoffset, double yoffset) {
			__scroll.x = static_cast<float>(xoffset);
			__scroll.y = static_cast<float>(yoffset);
		}

		inline static GLFWwindow* __window;
		inline static glm::vec2 __scroll;
		inline static bool __keyDown;
		inline static int __keyCount;
		inline static std::array<bool, GLFW_KEY_LAST + 1> __keyState;
		inline static std::array<short, GLFW_KEY_LAST + 1> __keyStateNow;
		inline static std::array<bool, GLFW_MOUSE_BUTTON_LAST + 1> __mouseButtonState;

		friend class Application;
	};

}
