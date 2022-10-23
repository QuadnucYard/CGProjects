#include <roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>

using namespace qy::cg;
std::shared_ptr<DisplayObject> createCylinder(int numDiv = 50)
{
	auto obj = DisplayObject().create();
	auto mr = obj->addComponent<MeshRenderer>();
	auto&& mesh = mr->getMesh();

	std::vector<glm::vec3> vertices(numDiv * 2 + 2);
	std::vector<GLuint> triangles(numDiv * 4 * 3);
	std::vector<glm::vec4> vcolors( numDiv * 2 + 2 , {1.0f, 1.0f, 1.0f, 1.0f} );
	vertices[numDiv] = { 0, 0, 0 };
	vertices[2 * numDiv + 1] = { 0, 0, 1 };

	using namespace std::numbers;
	for (int i = 0; i < numDiv; i++) {
		vertices[i] = glm::vec3(cos(pi * 2 * i / numDiv), sin(pi * 2 * i / numDiv), 0);
		triangles[i * 12 + 0] = numDiv;
		triangles[i * 12 + 1] = i % numDiv;
		triangles[i * 12 + 2] = (i + 1) % numDiv;

		vertices[numDiv + i] = glm::vec3(cos(pi * 2 * i / numDiv), sin(pi * 2 * i / numDiv), 1);
		triangles[i * 12 + 3] = numDiv * 2 + 1;
		triangles[i * 12 + 4] = i % numDiv + numDiv + 1;
		triangles[i * 12 + 5] = (i + 1) % numDiv + numDiv + 1;

		triangles[i * 12 + 6] = i % numDiv;
		triangles[i * 12 + 7] = i % numDiv + numDiv + 1;
		triangles[i * 12 + 8] = (i + 1) % numDiv + numDiv + 1;

		triangles[i * 12 + 9] = i % numDiv;
		triangles[i * 12 + 10] = (i + 1) % numDiv + numDiv + 1;
		triangles[i * 12 + 11] = (i + 1) % numDiv;
	}

	using namespace qy::cg;
	mesh.setVertices(vertices);
	mesh.setTriangles(triangles);
	mesh.setColors(vcolors);
	return obj;
	return obj;
}

class MyApplication : public qy::cg::Application {


private:
	std::shared_ptr<qy::cg::Scene> scene;
	std::shared_ptr<qy::cg::Camera> cam;
	glm::vec3 camFront{ 0, 0, -1 };
	glm::vec3 camUp{ 0, 1, 0 };

protected:
	void init() override {
		using namespace qy::cg;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		scene = Scene::create();
		cam = scene->createCamera();
		cam->obj()->transform()->position({ 1, 1, 1 });

		auto obj = Primitives::createCube();
		obj->transform()->position({ 0.5f, 0, 0 });
		obj->transform()->scale({ 0.3f, 0.3f, 0.3f });
		obj->transform()->rotation(glm::vec3(0.3f, 0.2f, 0.6f));
		obj->getComponent<MeshRenderer>()->getMesh().setColors(
			{ {0, 0, 0, 0}, {.5f, 0, 0, 1}, {0, .5f, 0, 1}, {0, 0, .5f, 1}, {.5f, .5f, 0, 1}, {.5f, 0, .5f, 1}, {0, .5f, .5f, 1}, {.5f, .5f, .5f, 1} });
		scene->root()->addChild(obj->transform());
		//
		//obj = Primitives::createCube();
		//obj->transform()->position({ 0.1f, 0.3f, -0.2f });
		//obj->transform()->scale({ 0.3f, 0.1f, 0.4f });
		//obj->transform()->rotation(glm::vec3(0.7f, -0.3f, 0.1f));
		//scene->root()->addChild(obj->transform());

		obj = createCylinder();
		obj->transform()->scale({ 0.1f, 0.1f, 0.1f });
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