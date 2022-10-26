#include <roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>

using namespace qy::cg;
std::shared_ptr<DisplayObject> createCylinder(int numDiv = 50)
{
	auto obj = DisplayObject::create();
	auto mr = obj->addComponent<MeshRenderer>();
	mr->setMaterial(Materials::geom_unlit);
	auto mf = obj->addComponent<MeshFilter>();
	auto mesh = mf->mesh();

	std::vector<glm::vec3> vertices(numDiv * 2 + 2);
	std::vector<GLuint> triangles(numDiv * 4 * 3);
	std::vector<glm::vec4> vcolors(numDiv * 2 + 2, { 1.0f, 1.0f, 1.0f, 1.0f });
	vertices[numDiv] = { 0, -1, 0 };
	vertices[2 * numDiv + 1] = { 0, 1, 0 };

	using namespace std::numbers;
	for (int i = 0; i < numDiv; i++) {
		vertices[i] = glm::vec3(cos(pi * 2 * i / numDiv), -1, sin(pi * 2 * i / numDiv));
		triangles[i * 12 + 0] = numDiv;
		triangles[i * 12 + 1] = i % numDiv;
		triangles[i * 12 + 2] = (i + 1) % numDiv;

		vertices[numDiv + i + 1] = glm::vec3(cos(pi * 2 * i / numDiv), 1, sin(pi * 2 * i / numDiv));
		triangles[i * 12 + 3] = numDiv * 2 + 1;
		triangles[i * 12 + 4] = (i + 1) % numDiv + numDiv + 1;
		triangles[i * 12 + 5] = i % numDiv + numDiv + 1;

		triangles[i * 12 + 6] = i % numDiv;
		triangles[i * 12 + 7] = i % numDiv + numDiv + 1;
		triangles[i * 12 + 8] = (i + 1) % numDiv + numDiv + 1;

		triangles[i * 12 + 9] = i % numDiv;
		triangles[i * 12 + 10] = (i + 1) % numDiv + numDiv + 1;
		triangles[i * 12 + 11] = (i + 1) % numDiv;
	}

	using namespace qy::cg;
	mesh->setVertices(vertices);
	mesh->setTriangles(triangles);
	mesh->setColors(vcolors);
	return obj;
}

glm::quat RotU2V(glm::vec3 start, glm::vec3 dest) {
	start = normalize(start);
	dest = normalize(dest);
	float cosTheta = dot(start, dest);
	glm::vec3 rotationAxis;

	rotationAxis = cross(start, dest);
	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;
	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}

class Car {
private:
	float m_speed;
	std::shared_ptr<DisplayObject> carObj;
	glm::vec3 carFront;
public:
	void init(float speed, glm::vec3 front) {
		m_speed = speed;
		carFront = front;

		carObj = DisplayObject::create();
		auto mr = carObj->addComponent<MeshRenderer>();
		mr->setMaterial(Materials::geom_unlit);
		auto mf = carObj->addComponent<MeshFilter>();

		auto color1 = Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.5f, Random::range(0.7f, 1.0f));
		auto color2 = Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.5f, Random::range(0.7f, 1.0f));
		auto color3 = Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.3f, Random::range(0.7f, 1.0f));
		auto color4 = Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.3f, Random::range(0.7f, 1.0f));
		auto color5 = Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.8f, Random::range(0.7f, 1.0f));

		std::vector<glm::vec4> vcolors(2 * 50 + 2);
		vcolors[50] = color3;
		vcolors[2 * 50 + 1] = color3;
		for (int i = 0; i < 25; i++) {
			vcolors[i] = color3;
			vcolors[25 + i] = color4;
			vcolors[51 + i] = color3;
			vcolors[76 + i] = color4;
		}

		auto obj = Primitives::createCube();
		obj->transform()->scale({ 0.2f, 0.05f, 0.3f });
		obj->getComponent<MeshFilter>()->mesh()->setColors({ 8,color1 });
		carObj->transform()->addChild(obj->transform());

		obj = Primitives::createCube();
		obj->transform()->scale({ 0.2f, 0.05f, 0.2f });
		obj->transform()->position({ 0.0f, 0.1f, 0.0f });
		obj->getComponent<MeshFilter>()->mesh()->setColors({ 8,color2 });
		carObj->transform()->addChild(obj->transform());

		for (int i = -1; i <= 1; i+=2) {
			for (int j = -1; j <= 1; j += 2) {
				obj = createCylinder();
				obj->transform()->scale({ 0.05, 0.02, 0.05 });
				obj->transform()->rotation(glm::vec3({ 0.0, 0.0, glm::radians(90.0) }));
				obj->transform()->position({ i*0.22, -0.05, j*0.15 });
				obj->getComponent<MeshFilter>()->mesh()->setColors(vcolors);
				carObj->transform()->addChild(obj->transform());
			}
		}

		obj = Primitives::createSphere();
		obj->transform()->scale({ 0.02, 0.02, 0.02 });
		obj->transform()->position({ 0.1, 0.0, 0.3 });
		obj->getComponent<MeshFilter>()->mesh()->setColors({ 51 * 51, color5 });
		carObj->transform()->addChild({ obj->transform() });
		obj = Primitives::createSphere();
		obj->transform()->scale({ 0.02, 0.02, 0.02 });
		obj->transform()->position({ -0.1, 0.0, 0.3 });
		obj->getComponent<MeshFilter>()->mesh()->setColors({ 51 * 51, color5 });
		carObj->transform()->addChild({ obj->transform() });
	}
	std::shared_ptr<DisplayObject> getObj() { return carObj; }
	glm::vec3 getCarFront() { return carFront; }
	float getSpeed() { return m_speed; }
};

class MyApplication : public qy::cg::Application {
private:
	std::shared_ptr<qy::cg::Scene> scene;
	std::shared_ptr<qy::cg::Camera> cam;
	glm::vec3 camFront{ 0, 0, -1 };
	glm::vec3 camUp{ 0, 1, 0 };

	glm::vec3 posLast = { 0.0f, 0.0f, 0.0f };
	bool mouseLeftPressed = false;
	float sensitivity = 0.1f;
	float yaw = -90.0f;
	float pitch = 0.0f;

	Car c;

protected:
	void init() override {
		using namespace qy::cg;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		scene = Scene::create();
		cam = scene->createCamera();
		cam->obj()->transform()->position({ 0, 0.3, 1 });

		c.init(0.05, {1.0, 0.0, 0.0});
		scene->root()->addChild(c.getObj()->transform());
	}

	void update() override {
		using namespace qy::cg;

		auto carPos = c.getObj()->transform()->position();
		carPos += c.getCarFront() * 

		scene->dispatch_update();

		float cameraSpeed = Time::deltaTime() * 1;
		auto camPos = cam->obj()->transform()->position();

		if (Input::getKeyDown(KeyCode::W)) camPos += cameraSpeed * camFront;
		if (Input::getKeyDown(KeyCode::S)) camPos -= cameraSpeed * camFront;
		if (Input::getKeyDown(KeyCode::A)) camPos -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
		if (Input::getKeyDown(KeyCode::D)) camPos += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
		if (Input::getKeyDown(KeyCode::SPACE)) camPos += camUp * cameraSpeed;
		if (Input::getKeyDown(KeyCode::LEFT_SHIFT)) camPos -= camUp * cameraSpeed;


		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && !mouseLeftPressed) {
			posLast = Input::mousePosition();
			mouseLeftPressed = true;
		}
		if (Input::getMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT)) {
			mouseLeftPressed = false;
		}
		if (Input::getMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			glm::vec3 posNow = Input::mousePosition();
			glm::vec3 posDelta = posNow - posLast;
			posLast = posNow;
			float xOffset = posDelta.x * sensitivity;
			float yOffset = -posDelta.y * sensitivity;
			yaw += xOffset;
			pitch += yOffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			camFront = glm::normalize(front);
		}

		cam->transform()->position(camPos);

		cam->transform()->rotation(RotU2V({ 0.0, 0.0, -1.0 }, camFront));
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