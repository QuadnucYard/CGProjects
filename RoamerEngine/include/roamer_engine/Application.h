#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string_view>
#include "display/Shaders.h"
#include "Time.h"

namespace qy::cg {

	class Application {

	public:
		static int VERSION_MAJOR;
		static int VERSION_MINOR;


	private:
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
		}

		~Application() {
			if (window) glfwDestroyWindow(window);
			glfwTerminate();
			exit(EXIT_SUCCESS);
		}

		Application* createWindow(int width, int height, std::string_view title) {
			window = glfwCreateWindow(800, 800, title.data(), nullptr, nullptr);
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
				glfwPollEvents(); // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
			}
			return this;
		}

	private:
		void internalInit() {
			Shaders::__INIT__();
		}

		void internalUpdate() {
			Time::__update(glfwGetTime());
		}

	protected:

		virtual void init() {}
		virtual void update() {}
		virtual void display() {}
	};
}

int qy::cg::Application::VERSION_MAJOR = 4;
int qy::cg::Application::VERSION_MINOR = 3;