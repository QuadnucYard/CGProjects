#include<roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>
#include"Maze.hpp"
#include"Wall.hpp"
#include"MoveControl.hpp"

namespace qy::cg {

	class MyApplication : public qy::cg::Application {
	private:
		std::shared_ptr<qy::cg::Scene> scene;
		std::shared_ptr<qy::cg::Camera> cam;
	protected:
		void init() override {
			using namespace qy::cg;
			/*glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);*/
			scene = Scene::create();
			cam = scene->createCamera();
			cam->setFarClipPlane(50.0);
			//cam->setBackgroundColor({ 0.4f, 0.3f, 0.1f, 1.0f });
			cam->obj()->transform()->position({ 0, 0, 0 });
			cam->addComponent(SkyBox::loadFromFile(
				"assets/skybox/right.jpg",
				"assets/skybox/left.jpg",
				"assets/skybox/top.jpg",
				"assets/skybox/bottom.jpg",
				"assets/skybox/front.jpg",
				"assets/skybox/back.jpg"
			));
			cam->setClearFlags(CameraClearFlags::Skybox);
			cam->addComponent<MoveControl>()->setFrontInit({ 0.0, 0.0, -1.0 });

			//设置光标不可见
			glfwSetInputMode(mainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			int width = 7;
			int height = 7;
			Maze m(width, height);
			auto maze = m.getMaze();
			for (int i = 1; i <= width; i++) {
				for (int j = 1; j <= height; j++) {
					if ((i == 1 && j == height - 1) || (i == width - 1 && j == 1)) {
						Wall w1, w2;
						w1.position({ i * 2.0, 2.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(w1.getObj()->transform());
						w2.position({ i * 2.0, -2.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(w2.getObj()->transform());
						continue;
					}
					if (maze[i][j] == -1) {
						Wall w;
						w.position({ i * 2.0, 0.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(w.getObj()->transform());
					}
					else {
						Wall w1, w2;
						w1.position({ i * 2.0, 2.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(w1.getObj()->transform());
						w2.position({ i * 2.0, -2.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(w2.getObj()->transform());
					}
				}
			}
		}

		void update() override {
			using namespace qy::cg;
			auto rot = glm::qua(glm::vec3({ 0.0, Time::time() , 0.0 }));
			scene->dispatch_update();
			cam->getComponent<MoveControl>()->update();
			if (Input::getKey(KeyCode::ESCAPE)) {
				if (mainWindow()) glfwDestroyWindow(mainWindow());
				glfwTerminate();
				exit(EXIT_SUCCESS);
			}
		}

		void display() override {
			scene->dispatch_render();
		}
	};
}

int main() {
	qy::cg::MyApplication app;
	app.createWindow(1280, 720, "MyApplication");
	app.run();
	return 0;
}
