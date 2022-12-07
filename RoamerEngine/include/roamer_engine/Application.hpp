#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <string_view>

namespace qy::cg {

	class Application {
	protected:
		inline static int VERSION_MAJOR = 4;
		inline static int VERSION_MINOR = 6;

		inline static Application* s_main;

		GLFWwindow* window;

	public:
		Application();

		~Application();

		Application* createWindow(int width, int height, std::string_view title);

		Application* run();

		void quit();

		inline static Application* main() {
			return s_main;
		}

		inline static GLFWwindow* mainWindow() {
			return s_main->window;
		}

	protected:
		virtual void internalInit();

		virtual void internalUpdate();

		virtual void internalRender() {}

		virtual void internalLateUpdate() {}

	protected:

		virtual void init() {}
		virtual void update() {}
		virtual void lateUpdate() {}
		virtual void display() {}
	};
}
