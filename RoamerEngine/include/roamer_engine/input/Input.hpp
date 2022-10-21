#pragma once
#include "../Application.hpp"
#include "KeyCode.hpp"

namespace qy::cg {

	class Input {

	public:

		static bool getKeyDown(KeyCode key) {
			return glfwGetKey(Application::mainWindow(), static_cast<int>(key)) == GLFW_PRESS;
		}

		static bool getKeyUp(KeyCode key) {
			return glfwGetKey(Application::mainWindow(), static_cast<int>(key)) == GLFW_RELEASE;
		}

		static glm::vec3 mousePosition() {
			double xpos, ypos;
			glfwGetCursorPos(Application::mainWindow(), &xpos, &ypos);
			return {xpos, ypos, 0};
		}

		static bool getMouseButtonDown(int button) {
			return glfwGetMouseButton(Application::mainWindow(), button) == GLFW_PRESS;
		}

		static bool getMouseButtonUp(int button) {
			return glfwGetMouseButton(Application::mainWindow(), button) == GLFW_RELEASE;
		}
	};

}
