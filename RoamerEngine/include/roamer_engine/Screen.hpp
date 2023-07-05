#pragma once
#include "Rect.hpp"

struct GLFWwindow;

namespace qy::cg {

	class Screen {

	public:
		static void __init__(GLFWwindow* window);

		static int width() { return _width; }
		static int height() { return _height; }

		static bool isFullScreen() {
			return _fullScreen;
		}

		static void setFullScreen(bool value);

	private:
		static void cbFramebufferSize(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
		}

		static void cbWindowSize(GLFWwindow* window, int width, int height) {
			_width = width;
			_height = height;
		}

		static void cbWindowPos(GLFWwindow* window, int xpos, int ypos) {
			_xpos = xpos;
			_ypos = ypos;
		}

	private:
		inline static GLFWwindow* _window;
		inline static int _xpos;
		inline static int _ypos;
		inline static int _width;
		inline static int _height;
		inline static bool _fullScreen;
	};
}