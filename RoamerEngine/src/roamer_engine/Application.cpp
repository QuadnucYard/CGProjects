#include "roamer_engine/Application.hpp"
#include "roamer_engine/display/Materials.hpp"
#include "roamer_engine/display/Shaders.hpp"
#include "roamer_engine/input/Input.hpp"
#include "roamer_engine/input/Cursor.hpp"
#include "roamer_engine/Screen.hpp"
#include "roamer_engine/Time.hpp"

namespace qy::cg {

	Application::Application() {
		if (!glfwInit()) {
			std::exit(EXIT_FAILURE);
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
		window = nullptr;
		s_main = this;
	}

	Application::~Application() {
		if (window) glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}

	Application* Application::createWindow(int width, int height, std::string_view title) {
		window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
		glfwMakeContextCurrent(window);
		Screen::__update__(width, height);
		//设置缓存刷新时间
		glfwSwapInterval(1);
		//glad初始化
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize GLAD");
			exit(EXIT_FAILURE);
		}
		return this;
	}

	Application* Application::run() {
		internalInit();
		init();
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
			internalUpdate();
			update();
			internalRender();
			display();
			internalLateUpdate();
			lateUpdate();
			glfwSwapBuffers(window);
		}
		return this;
	}

	void Application::internalInit() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		Screen::__init__(window);
		Input::__init__(window);
		Cursor::__init__(window);
		Shaders::__INIT__();
		Materials::__init__();
	}

	void Application::internalUpdate() {
		Time::__update(static_cast<float>(glfwGetTime()));
		Input::__update__(); // Input状态重置
	}
}