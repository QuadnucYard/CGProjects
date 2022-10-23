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
			//glfw��ʼ��
			if (!glfwInit()) {
				std::exit(EXIT_FAILURE);
			}
			//���ڰ汾
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR); //���汾��
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR); //�ΰ汾��
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
			//���û���ˢ��ʱ��
			glfwSwapInterval(1);
			//glew��ʼ��
			if (glewInit() != GLEW_OK) {
				exit(EXIT_FAILURE);
			}
			return this;
		}

		Application* run() {
			internalInit();
			init();
			//�¼�ѭ�������������¼�
			while (!glfwWindowShouldClose(window)) {
				internalUpdate();
				update();
				display(); //���ƺ���������
				glfwSwapBuffers(window); //������ɫ����
				internalLateUpdate();
				glfwPollEvents(); // �����û�д���ʲô�¼�������������롢����ƶ��ȣ�
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
			Input::__update__(); // Input״̬����
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