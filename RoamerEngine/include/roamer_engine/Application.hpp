#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string_view>
#include "display/Shaders.hpp"
#include "display/Materials.hpp"
#include "Time.hpp"
#include "input/Input.hpp"

namespace qy::cg {

	class Application {
	public:
		static int VERSION_MAJOR;
		static int VERSION_MINOR;

	private:
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
			//设置缓存刷新时间
			glfwSwapInterval(1);
			//glew初始化
			if (glewInit() != GLEW_OK) {
				exit(EXIT_FAILURE);
			}
			return this;
		}

		Application* run() {
			internalInit();
			init();
			//事件循环，接收输入事件
			while (!glfwWindowShouldClose(window)) {
				internalUpdate();
				update();
				display(); //绘制函数，主体
				glfwSwapBuffers(window); //交换颜色缓存
				internalLateUpdate();
				glfwPollEvents(); // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
				lateUpdate();
			}
			return this;
		}

		static Application* main() {
			return s_main;
		}

		static GLFWwindow* mainWindow() {
			return s_main->window;
		}

	private:
		void internalInit() {
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
			Input::__init__(window);
			Shaders::__INIT__();
			Materials::__init__();
		}

		void internalUpdate() {
			Time::__update(glfwGetTime());
		}

		void internalLateUpdate() {
			Input::__update__(); // Input状态重置
		}

	protected:

		virtual void init() {}
		virtual void update() {}
		virtual void lateUpdate() {}
		virtual void display() {}
	};
}

int qy::cg::Application::VERSION_MAJOR = 4;
int qy::cg::Application::VERSION_MINOR = 3;