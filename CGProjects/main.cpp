#include <roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>

class MyApplication: public qy::cg::Application {

	
private:
	std::shared_ptr<qy::cg::Scene> scene;
	std::shared_ptr<qy::cg::Camera> cam;
	glm::vec3 camFront {0, 0, -1};
	glm::vec3 camUp {0, 1, 0};

protected:
	void init() override {
		using namespace qy::cg;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		scene = Scene::create();
		cam = scene->createCamera();
		cam->obj()->transform()->position({1, 1, 1});
		
		auto obj = Primitives::createCube();
		obj->transform()->position({0.5f, 0, 0});
		obj->transform()->scale({0.3f, 0.3f, 0.3f});
		obj->transform()->rotation(glm::vec3(0.3f, 0.2f, 0.6f));
		obj->getComponent<MeshRenderer>()->getMesh().setColors(
			{{0, 0, 0, 0}, {.5f, 0, 0, 1}, {0, .5f, 0, 1}, {0, 0, .5f, 1}, {.5f, .5f, 0, 1}, {.5f, 0, .5f, 1}, {0, .5f, .5f, 1}, {.5f, .5f, .5f, 1}});
		scene->root()->addChild(obj->transform());
		obj = Primitives::createCube();
		obj->transform()->position({0.1f, 0.3f, -0.2f});
		obj->transform()->scale({0.3f, 0.1f, 0.4f});
		obj->transform()->rotation(glm::vec3(0.7f, -0.3f, 0.1f));
		scene->root()->addChild(obj->transform());
	}

	void update() override {
		using namespace qy::cg;
		scene->dispatch_update();
		
		float cameraSpeed = Time::deltaTime() * 5;
		auto camPos = cam->obj()->transform()->position();
		if (Input::getKeyDown(KeyCode::W)) camPos += cameraSpeed * camFront;
		if (Input::getKeyDown(KeyCode::S)) camPos -= cameraSpeed * camFront;
		if (Input::getKeyDown(KeyCode::A)) camPos -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
		if (Input::getKeyDown(KeyCode::D)) camPos += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
		if (Input::getKeyDown(KeyCode::SPACE)) camPos += camUp * cameraSpeed;
		if (Input::getKeyDown(KeyCode::LEFT_SHIFT)) camPos -= camUp * cameraSpeed;
		cam->transform()->position(camPos);
		cam->setFieldOfView(std::clamp(cam->getFieldOfView() + Input::mouseScrollDelta().y, 1.0f, 80.0f));
	}

	void display() override {
		scene->dispatch_render();
	}
};

int main() {
	MyApplication app;
	app.createWindow(800, 800, "MyApplication");
	app.run();
	return 0;
}