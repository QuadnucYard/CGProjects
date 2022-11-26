#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string_view>
#include "display/Shaders.hpp"
#include "display/Materials.hpp"
#include "Time.hpp"
#include "Screen.hpp"
#include "input/Input.hpp"

namespace qy::cg {

	class Application {
	public:
		static int VERSION_MAJOR;
		static int VERSION_MINOR;

	protected:
		inline static Application* s_main;

		GLFWwindow* window;

	public:
		Application() {
			//glfw初始化
			if (!glfwInit()) {
				std::exit(EXIT_FAILURE);
			}
			//窗口版本
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR); //主版本号
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR); //次版本号
			window = nullptr;
			s_main = this;
		}

		~Application() {
			if (window) glfwDestroyWindow(window);
			glfwTerminate();
			exit(EXIT_SUCCESS);
		}

		Application* createWindow(int width, int height, std::string_view title) {
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

		Application* run() {
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

		static Application* main() {
			return s_main;
		}

		static GLFWwindow* mainWindow() {
			return s_main->window;
		}

	protected:
		virtual void internalInit() {
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			Screen::__init__(window);
			Input::__init__(window);
			Shaders::__INIT__();
			Materials::__init__();
		}

		virtual void internalUpdate() {
			Time::__update(glfwGetTime());
			Input::__update__(); // Input状态重置
		}

		virtual void internalRender() {}

		virtual void internalLateUpdate() {}

	protected:

		virtual void init() {}
		virtual void update() {}
		virtual void lateUpdate() {}
		virtual void display() {}
	};
}

int qy::cg::Application::VERSION_MAJOR = 4;
int qy::cg::Application::VERSION_MINOR = 3;