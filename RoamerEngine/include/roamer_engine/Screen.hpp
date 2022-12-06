#pragma once
#include <GLFW/glfw3.h>

namespace qy::cg {

	class Screen {

	public:
		static int width() { return _width; }
		static int height() { return _height; }

		static void __init__(GLFWwindow* window) {
			glfwSetFramebufferSizeCallback(window, cbFramebufferSize);
		}

		static void __update__(int width, int height) {
			_width = width;
			_height = height;
		}

	private:
		static void cbFramebufferSize(GLFWwindow* window, int width, int height) {
			_width = width;
			_height = height;
			glViewport(0, 0, width, height);
		}

	private:
		inline static int _width;
		inline static int _height;
	};
}