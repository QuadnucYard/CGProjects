#include<roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>
#include"Maze.hpp"
#include"Wall.hpp"
#include"MoveControl.hpp"

namespace qy::cg {

	class TopLight {
	private:
		ptr<DisplayObject> obj;
	public:
		TopLight() {
			static int instance_cnt = 0;
			obj = Primitives::createSphere();
			obj->transform()->scale({ 0.1,0.1,0.05 });
			obj->transform()->rotation(glm::vec3({ glm::radians(-90.0), 0.0, 0.0 }));
			obj->addComponent<MeshRenderer>()->setMaterial(Materials::GeomUnlit);
			
			auto&& light = obj->addComponent<Light>();
			light->setType(LightType::Spot);
			light->setAmbient({ 0.05f, 0.05f, 0.05f, 1.0f });
			light->setDiffuse({ 0.9f, 0.4f, 0.8f, 1.0f });
			light->setSpecular({ 0.2f, 0.3f, 0.5f, 1.0f });
			light->setIntensity(1.0f);
			light->setRange(100);
			light->setSpotAngle(15);

			if (instance_cnt++ == 0) {
				auto&& mat = obj->getComponent<MeshRenderer>()->getSharedMaterial();
				mat->setShader(Shaders::GeomUnlit);
				mat->setColor({ 0.9f, 0.4f, 0.8f, 1.0f });
			}
		}
		ptr<DisplayObject> getObj() { return obj; }
	};

	class MyApplication : public qy::cg::Application {
	private:
		std::shared_ptr<qy::cg::Scene> scene;
		std::shared_ptr<qy::cg::Camera> cam;
		ptr<DisplayObject> obj;
	protected:
		void init() override {
			using namespace qy::cg;
			/*glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);*/
			scene = Scene::create();
			cam = scene->createCamera();
			cam->setFarClipPlane(100);
			//cam->setBackgroundColor({ 0.4f, 0.3f, 0.1f, 1.0f });
			cam->obj()->transform()->position({ 0, 0, 0 });
			cam->obj()->transform()->rotation(glm::vec3({ 0.0, glm::radians(-90.0), 0.0 }));
			cam->addComponent(SkyBox::loadFromFile(
				"assets/skybox/right.jpg",
				"assets/skybox/left.jpg",
				"assets/skybox/top.jpg",
				"assets/skybox/bottom.jpg",
				"assets/skybox/front.jpg",
				"assets/skybox/back.jpg"
			));
			cam->setClearFlags(CameraClearFlags::Skybox);
			cam->addComponent<MoveControl>()->init({ 1.0, 0.0, 0.0 });

			//���ù�겻�ɼ�
			glfwSetInputMode(mainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			auto&& light = cam->addComponent<Light>();
			light->setType(LightType::Spot);
			light->setAmbient({ 0.05f, 0.05f, 0.05f, 1.0f });
			light->setDiffuse(Color::rgba(250, 250, 236));
			light->setSpecular(Color::rgba(250, 250, 236));
			light->setIntensity(0.2f);
			light->setRange(100);
			light->setSpotAngle(15);

			int width = 11;
			int height = 11;
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
						TopLight light;
						light.getObj()->transform()->position({ i * 2.0, 1.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(light.getObj()->transform());
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
						TopLight light;
						light.getObj()->transform()->position({ i * 2.0, 1.0, (j - height + 1) * 2.0 });
						scene->root()->addChild(light.getObj()->transform());
					}
				}
			}
		}

		void update() override {
			using namespace qy::cg;
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