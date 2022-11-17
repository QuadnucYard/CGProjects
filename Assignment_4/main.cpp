﻿#include<roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>
#include"Maze.hpp"
#include"Wall.hpp"
#include"MoveControl.hpp"

using namespace qy::cg;

class Spinning: public Component {
public:
	void update() override {
		transform()->rotation(glm::vec3 {0.0, Time::time() * 10, 0.0});
	}
};

class Flicker: public Component {
private:
	ptr<Light> light;
	double offset;
public:
	void start() override {
		light = getComponent<Light>();
		offset = Random::range(3.0f);
	}
	void update() override {
		light->setIntensity(sin(tan(Time::time() + offset)));
	}
};

class TopLight {
private:
	ptr<DisplayObject> obj;
public:
	TopLight() {
		obj = Primitives::createSphere();
		obj->transform()->scale({0.1, 0.1, 0.05});
		obj->transform()->rotation(glm::vec3({glm::radians(-90.0), 0.0, 0.0}));

		auto color = Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.5f, 1.0);
		auto&& light = obj->addComponent<Light>();
		light->setType(LightType::Spot);
		light->setAmbient({0.0f, 0.0f, 0.0f, 1.0f});
		light->setDiffuse(color);
		light->setSpecular(color);
		light->setIntensity(1.0f);
		light->setRange(30);
		light->setInnerSpotAngle(20);
		light->setSpotAngle(60);

		obj->getComponent<MeshRenderer>()->getMaterial()->setColor(color);
		obj->addComponent<Flicker>();
	}
	ptr<DisplayObject> getObj() { return obj; }
};

class MyApplication: public Application {
private:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Camera> cam;
	ptr<DisplayObject> obj, monkey;
protected:
	void init() override {
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);*/
		scene = Scene::create();
		cam = scene->createCamera();
		cam->setFarClipPlane(100);
		//cam->setBackgroundColor({ 0.4f, 0.3f, 0.1f, 1.0f });
		cam->obj()->transform()->position({0, 0, 0});
		cam->obj()->transform()->rotation(glm::vec3({0.0, glm::radians(-90.0), 0.0}));
		cam->addComponent(SkyBox::loadFromFile(
			"assets/skybox/right.jpg",
			"assets/skybox/left.jpg",
			"assets/skybox/top.jpg",
			"assets/skybox/bottom.jpg",
			"assets/skybox/front.jpg",
			"assets/skybox/back.jpg"
		));
		cam->setClearFlags(CameraClearFlags::Skybox);
		cam->addComponent<MoveControl>();

		//设置光标不可见
		glfwSetInputMode(mainWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		auto&& light = cam->addComponent<Light>();
		light->setType(LightType::Spot);
		light->setAmbient({0.1f, 0.1f, 0.1f, 1.0f});
		light->setDiffuse(Color::rgba(250, 250, 236));
		light->setSpecular(Color::rgba(250, 250, 236));
		light->setIntensity(1.0f);
		light->setRange(20);
		light->setInnerSpotAngle(20);
		light->setSpotAngle(60);

		int width = 7;
		int height = 7;
		Maze m(width, height);
		auto maze = m.getMaze();
		for (int i = 1; i <= width; i++) {
			for (int j = 1; j <= height; j++) {
				if ((i == 1 && j == height - 1)) {
					Wall w1, w2;
					w1.position({i * 2.0, 2.0, (j - height + 1) * 2.0});
					scene->root()->addChild(w1.getObj()->transform());
					w2.position({i * 2.0, -2.0, (j - height + 1) * 2.0});
					scene->root()->addChild(w2.getObj()->transform());
					if (i % 2 == 0 && j % 2 == 0) {
						TopLight light;
						light.getObj()->transform()->position({i * 2.0, 1.0, (j - height + 1) * 2.0});
						scene->root()->addChild(light.getObj()->transform());
					}
					continue;
				}

				if (i == width - 1 && j == 2) {
					monkey = ModelLoader::loadObj("assets/ApexPlasmaMasterGeo.obj");
					monkey->transform()->position({i * 2.0, -1.0, (j - height + 1) * 2.0});
					monkey->transform()->scale({0.03f, 0.03f, 0.03f});
					monkey->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Texture::loadFromFile("assets/ApexPlasmaMasterDiffuse.png"));
					scene->root()->addChild(monkey->transform());
					monkey->addComponent<Spinning>();
				}

				if (maze[i][j] == -1) {
					Wall w;
					w.position({i * 2.0, 0.0, (j - height + 1) * 2.0});
					scene->root()->addChild(w.getObj()->transform());
				} else {
					Wall w1, w2;
					w1.position({i * 2.0, 2.0, (j - height + 1) * 2.0});
					scene->root()->addChild(w1.getObj()->transform());
					w2.position({i * 2.0, -2.0, (j - height + 1) * 2.0});
					scene->root()->addChild(w2.getObj()->transform());
					if (i % 2 == 0 && j % 2 == 0) {
						TopLight light;
						light.getObj()->transform()->position({i * 2.0, 1.0, (j - height + 1) * 2.0});
						scene->root()->addChild(light.getObj()->transform());
					}
				}
			}
		}

		{
			auto darkCastle = ModelLoader::loadObj("assets/DarkCastle.obj");
			darkCastle->transform()->position({-10, -5, -5});
			darkCastle->transform()->scale(glm::vec3 {1.0f});
			darkCastle->transform()->rotation(glm::radians(glm::vec3 {0, -90, 0}));
			darkCastle->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Texture::loadFromFile("assets/DarkCastleDiffuse.png"));
			scene->root()->addChild(darkCastle->transform());
		}
		{
			auto cornfield = ModelLoader::loadObj("assets/CornField.obj");
			cornfield->transform()->position({1, 1, 0});
			//cornfield->transform()->scale(glm::vec3 {1.0f});
			//cornfield->transform()->rotation(glm::radians(glm::vec3 {0, -90, 0}));
			cornfield->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Texture::loadFromFile("assets/CornfieldDiffuse.png"));
			scene->root()->addChild(cornfield->transform());
		}
		{
			auto model = ModelLoader::loadObj("assets/Lotus.obj");
			model->transform()->position({3, -1, 0});
			model->transform()->scale(glm::vec3 {0.1f});
			//cornfield->transform()->rotation(glm::radians(glm::vec3 {0, -90, 0}));
			model->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Texture::loadFromFile("assets/LotusTree.png"));
			scene->root()->addChild(model->transform());
		}
		{
			auto obj = DisplayObject::create();
			obj->transform()->position({0, 10, 0});
			auto&& light = obj->addComponent<Light>();
			light->setType(LightType::Point);
			light->setAmbient({0.1f, 0.1f, 0.1f, 1.0f});
			light->setDiffuse(Color::rgba(250, 250, 250));
			//light->setSpecular(Color::rgba(250, 250, 236));
			light->setIntensity(1.0f);
			light->setRange(100);
			//light->setInnerSpotAngle(90);
			//light->setSpotAngle(90);
			scene->root()->addChild(obj->transform());
		}
	}

	void update() override {
		using namespace qy::cg;

		auto range = cam->getComponent<Light>()->getRange();
		auto innerAngle = cam->getComponent<Light>()->getInnerSpotAngle();
		if (Input::getKey(KeyCode::EQUAL)) {
			if (range < 1000) {
				range++;
				cam->getComponent<Light>()->setRange(range);
			}
		}
		if (Input::getKey(KeyCode::MINUS)) {
			if (range > 1) {
				range--;
				cam->getComponent<Light>()->setRange(range);
			}
		}

		auto angle = cam->getComponent<Light>()->getSpotAngle();
		if (Input::getKey(KeyCode::RIGHT_BRACKET)) {
			if (angle < 90) {
				angle++;
				cam->getComponent<Light>()->setSpotAngle(angle);
			}
		}
		if (Input::getKey(KeyCode::LEFT_BRACKET)) {
			if (angle > innerAngle) {
				angle--;
				cam->getComponent<Light>()->setSpotAngle(angle);
			}
		}

		monkey->transform()->rotation(glm::vec3(0.0, glm::radians(Time::deltaTime()), 0.0));

		scene->dispatch_update();
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


int main() {
	MyApplication app;
	app.createWindow(1280, 720, "MyApplication");
	app.run();
	return 0;
}
