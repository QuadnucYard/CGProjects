#include <roamer_engine.hpp>
#include <roamer_editor.hpp>
#include <cmath>
#include <numbers>
#include <random>


class MyApplication: public qy::cg::editor::EditorApplication {


private:
	std::shared_ptr<qy::cg::Scene> scene;
	std::shared_ptr<qy::cg::Camera> cam;
	glm::vec3 camFront {0, 0, -1};
	glm::vec3 camUp {0, 1, 0};

protected:
	void init() override {
		using namespace qy::cg;
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);*/
		scene = Scene::create();
		cam = scene->createCamera();
		cam->setBackgroundColor({0.4f, 0.3f, 0.1f, 1.0f});
		cam->obj()->transform()->position({0, 0.5f, 5});
		cam->addComponent(SkyBox::loadFromFile(
			"assets/skybox/right.jpg",
			"assets/skybox/left.jpg",
			"assets/skybox/top.jpg",
			"assets/skybox/bottom.jpg",
			"assets/skybox/front.jpg",
			"assets/skybox/back.jpg"
		));
		cam->setClearFlags(CameraClearFlags::Skybox);

		{
			auto lightObj = Primitives::createSphere();
			lightObj->transform()->scale({0.1f, 0.1f, 0.1f});
			scene->root()->addChild(lightObj->transform());
			lightObj->transform()->position({1, 4, 1});
			auto&& light = lightObj->addComponent<Light>();
			light->setType(LightType::Point);
			light->setAmbient({0.5f, 0.0f, 0.0f, 1.0f});
			light->setDiffuse({1.0f, 0.1f, 0.1f, 1.0f});
			light->setSpecular({1.0f, 1.0f, 1.0f, 1.0f});
			light->setRange(100);
			light->setShadows(LightShadow::Soft);
		}
		{
			auto lightObj = Primitives::createSphere();
			lightObj->transform()->scale({0.1f, 0.1f, 0.1f});
			scene->root()->addChild(lightObj->transform());
			lightObj->transform()->position({-1, 3, 1});
			auto&& light = lightObj->addComponent<Light>();
			light->setType(LightType::Point);
			light->setAmbient({0.0f, 0.5f, 0.0f, 1.0f});
			light->setDiffuse({0.1f, 1.0f, 0.1f, 1.0f});
			light->setSpecular({1.0f, 1.0f, 1.0f, 1.0f});
			light->setRange(100);
			light->setShadows(LightShadow::Hard);
			auto&& obj222 = DisplayObject::create("test");
			lightObj->transform()->addChild(obj222->transform());
		}
		{
			auto lightObj = Primitives::createSphere();
			lightObj->transform()->scale({0.1f, 0.1f, 0.1f});
			scene->root()->addChild(lightObj->transform());
			lightObj->transform()->position({0, 2, -1});
			auto&& light = lightObj->addComponent<Light>();
			light->setType(LightType::Point);
			light->setAmbient({0.0f, 0.0f, 0.5f, 1.0f});
			light->setDiffuse({0.1f, 0.1f, 1.0f, 1.0f});
			light->setSpecular({1.0f, 1.0f, 1.0f, 1.0f});
			light->setRange(100);
			light->setShadows(LightShadow::Soft);
			auto&& obj222 = DisplayObject::create("test");
			lightObj->transform()->addChild(obj222->transform());
		}
		//scene->setAmbientColor({0.2f, 0.2f, 0.2f, 1.0f});
		{
			auto obj = Primitives::createCube();
			obj->transform()->position({0, -1, 0});
			obj->transform()->scale({10, 1, 10});
			//obj->getComponent<MeshRenderer>()->getMaterial()->setColor("material.ambient", {0.1f, 0.1f, 0.1f, 1.0f});
			obj->getComponent<MeshRenderer>()->getMaterial()->setTexture("_SpecTex", Assets::load<Texture2D>("assets/ApexPlasmaMasterDiffuse.png"));
			scene->root()->addChild(obj->transform());
		}
		{
			auto&& obj2 = ModelLoader::loadObj("assets/ApexPlasmaMasterGeo.obj");
			obj2->transform()->scale({0.05f, 0.05f, 0.05f});
			obj2->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Assets::load<Texture2D>("assets/ApexPlasmaMasterDiffuse.png"));
			scene->root()->addChild(obj2->transform());
			obj2->name("35454");
			auto&& obj222 = DisplayObject::create("test");
			obj2->transform()->addChild(obj222->transform());
		}
	}

	void update() override {
		using namespace qy::cg;
		scene->dispatch_update();

		float cameraSpeed = (float)Time::deltaTime() * 5;
		auto camPos = cam->obj()->transform()->position();
		if (Input::getKey(KeyCode::W)) camPos += cameraSpeed * camFront;
		if (Input::getKey(KeyCode::S)) camPos -= cameraSpeed * camFront;
		if (Input::getKey(KeyCode::A)) camPos -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
		if (Input::getKey(KeyCode::D)) camPos += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
		if (Input::getKey(KeyCode::SPACE)) camPos += camUp * cameraSpeed;
		if (Input::getKey(KeyCode::LEFT_SHIFT)) camPos -= camUp * cameraSpeed;
		cam->transform()->position(camPos);
		cam->setFieldOfView(std::clamp(cam->getFieldOfView() + Input::mouseScrollDelta().y, 1.0f, 80.0f));

		for (auto&& child : scene->root()) {
			auto&& mr = child->getComponent<MeshRenderer>();
			if (mr) {
				mr->getMaterial()->setFloat("_Time", Time::time());
			}
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