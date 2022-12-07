#pragma once
#include <GLFW/glfw3.h>
#include "Rect.hpp"

namespace qy::cg {

	class Screen {

	public:

		static void __init__(GLFWwindow* window) {
			_window = window;
			glfwSetFramebufferSizeCallback(window, cbFramebufferSize);
			glfwSetWindowSizeCallback(window, cbWindowSize);
			glfwSetWindowPosCallback(window,cbWindowPos );
			glfwGetWindowPos(window, &_xpos, &_ypos);
			glfwGetWindowSize(window, &_width, &_height);
		}

		static int width() { return _width; }
		static int height() { return _height; }

		static bool isFullScreen() {
			return _fullScreen;
		}

		static void setFullScreen(bool value) {
			static recti savedRect;
			if (value == _fullScreen) return;
			auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			if (_fullScreen = value) {
				savedRect = {_xpos, _ypos, _width, _height};
				glfwSetWindowMonitor(_window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
				// 这样有点问题  改了显示器
				//glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, Screen::width(), Screen::height(), mode->refreshRate);
			} else {
				glfwSetWindowMonitor(_window, nullptr, savedRect.x(), savedRect.y(), savedRect.width(), savedRect.height(), mode->refreshRate);
			}
		}

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