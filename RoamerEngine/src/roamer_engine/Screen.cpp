#include "roamer_engine/Screen.hpp"
#include <GLFW/glfw3.h>

namespace qy::cg {

	void Screen::__init__(GLFWwindow* window) {
		_window = window;
		glfwSetFramebufferSizeCallback(window, cbFramebufferSize);
		glfwSetWindowSizeCallback(window, cbWindowSize);
		glfwSetWindowPosCallback(window, cbWindowPos);
		glfwGetWindowPos(window, &_xpos, &_ypos);
		glfwGetWindowSize(window, &_width, &_height);
	}

	void Screen::setFullScreen(bool value) {
		static recti savedRect;
		if (value == _fullScreen)
			return;
		auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		if (_fullScreen = value) {
			savedRect = {_xpos, _ypos, _width, _height};
			glfwSetWindowMonitor(_window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);
			// 这样有点问题  改了显示器
			//glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, Screen::width(), Screen::height(), mode->refreshRate);
		} else {
			glfwSetWindowMonitor(_window, nullptr, savedRect.x(), savedRect.y(), savedRect.width(), savedRect.height(),
								 mode->refreshRate);
		}
	}

} // namespace qy::cg