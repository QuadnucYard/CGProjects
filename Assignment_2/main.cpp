#include <roamer_engine.h>
#include <cmath>
#include <numbers>
#include <random>

class MyApplication: public qy::cg::Application {

private:
	static constexpr int numDiv = 36;
	static constexpr float r1 = 0.4f, r2 = 0.7f;

	qy::cg::DisplayObject* obj1;
	qy::cg::DisplayObject* obj2;

protected:
	void init() override {
		obj1 = draw_Doraemon();
		obj2 = draw_YiJianSanLian();
		obj2->transform()->position() = {0, -0.5f, 0};
		obj2->transform()->scale() = {0.5f, 0.4f, 0.5f};
		obj2->transform()->rotation() = glm::radians(glm::vec3 {0.0f, 0.0f, 30.0f});
	}

	void update() override {
		using namespace qy::cg;
		obj1->transform()->scale() = {cos(qy::cg::Time::time() * 3) * 1.5f, sin(Time::time() * 5) * 2, 1};
		obj2->transform()->rotation() = glm::radians(glm::vec3 {0.0f, 0.0f, 480.0f * Time::time()});
		obj2->transform()->position() = {sin(Time::time() * 3) * 2.0f, 0.0f, 0.0f};

		for (auto c : obj2->getComponents<MeshRenderer>()) {
			auto colors = c->getMesh().getColors();
			std::ranges::transform(colors, colors.begin(), [=](auto c) {
				auto hsv = Color::rgb2hsv(c);
				hsv.r += 0.005f;
				auto rgb = Color::hsv2rgb(hsv);
				return rgb;
			});
			c->getMesh().setColors(colors);
		}
	}

	void display() override {
		glClearColor(0.5f, 0.8f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		obj1->render();
		obj2->render();
	}

	void addCircle(qy::cg::DisplayObject* obj, glm::vec3 center, float r, glm::vec4 color);

	void addFivePointedStar(qy::cg::DisplayObject* obj, glm::vec3 center, float r1, glm::vec4 color, float theta = 0, float r2 = 0);

	qy::cg::DisplayObject* draw_Doraemon();

	qy::cg::DisplayObject* draw_YiJianSanLian();
};

qy::cg::DisplayObject* MyApplication::draw_Doraemon() {
	using namespace qy::cg;
	auto obj = new DisplayObject();

	glm::vec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};
	glm::vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	glm::vec4 black = {0.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 red = {1.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 yellow = {1.0, 1.0f, 0.0f, 1.0f};

	addCircle(obj, {0.0f, 0.425f, 0.0f}, 0.425, blue);
	addCircle(obj, {0.0f, 0.3f, 0.0f}, 0.3, white);
	addCircle(obj, {-0.1f, 0.55f, 0.0f}, 0.096f, black);
	addCircle(obj, {0.1f, 0.55f, 0.0f}, 0.096f, black);
	addCircle(obj, {-0.1f, 0.55f, 0.0f}, 0.093f, white);
	addCircle(obj, {0.1f, 0.55f, 0.0f}, 0.093f, white);
	addCircle(obj, {0.04f, 0.55f, 0.0f}, 0.015f, black);
	addCircle(obj, {-0.04f, 0.55f, 0.0f}, 0.015f, black);
	addCircle(obj, {0.0f, 0.45f, 0.0f}, 0.05f, red);
	obj->addComponent<LineRenderer>()
		->setPositions({{0.0f, 0.15f, 0.0f}, {0.0f, 0.4f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({{0.1f, 0.35f, 0.0f}, {0.225f, 0.4f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({{0.1f, 0.3f, 0.0f}, {0.235f, 0.3f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({{0.1f, 0.25f, 0.0f}, {0.215f, 0.2f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({{-0.1f, 0.35f, 0.0f}, {-0.225f, 0.4f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({{-0.1f, 0.3f, 0.0f}, {-0.235f, 0.3f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({{-0.1f, 0.25f, 0.0f}, {-0.215f, 0.2f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);

	auto&& mesh = obj->addComponent<MeshRenderer>()->getMesh();
	mesh.setVertices({{-0.274f, 0.1f, 0.0f}, {-0.274f, 0.0f, 0.0f}, {0.274f, 0.0f, 0.0f}, {0.274f, 0.1f, 0.0f}});
	mesh.setColors({red, red, red, red});
	mesh.setTriangles({0, 1, 2, 0, 2, 3});

	addCircle(obj, {0.0f, -0.025f, 0.0f}, 0.0725f, black);
	addCircle(obj, {0.0f, -0.025f, 0.0f}, 0.07f, yellow);

	obj->addComponent<LineRenderer>()
		->setPositions({{-0.072f, -0.025f, 0.0f}, {0.072f, -0.025f, 0.0f}})
		->setColors({black, black})
		->setWidth(1);

	return obj;
}

qy::cg::DisplayObject* MyApplication::draw_YiJianSanLian() {
	using namespace qy::cg;
	auto obj = new DisplayObject();

	auto meshRenderer = obj->addComponent<MeshRenderer>();
	auto&& mesh = meshRenderer->getMesh();
	mesh.setVertices({{-0.925f, 0.2f, 0.0f}, {-0.925f, -0.2f, 0.0f}, {-0.98f, 0.18f, 0.0f}, {-0.98f, -0.18f, 0.0f}});
	glm::vec4 color = {0.0f, 161.0f / 255, 214.0f / 255, 1.0f};
	std::vector<glm::vec4> vcolors(4, color);
	mesh.setColors(vcolors);
	mesh.setTriangles({0, 2, 3, 0, 3, 1});

	auto meshRenderer2 = obj->addComponent<MeshRenderer>();
	auto&& mesh2 = meshRenderer2->getMesh();
	mesh2.setVertices({
		{-0.65f, 0.2f, 0.0f},
		{-0.35f, 0.2f, 0.0f},
		{-0.45f, -0.2f, 0.0f},
		{-0.9f, -0.2f, 0.0f},
		{-0.9f, 0.2f, 0.0f},
		{-0.8f, 0.3f, 0.0f},
		{-0.75f, 0.45f, 0.0f},
		{-0.65f, 0.45f, 0.0f},
		});
	vcolors = std::vector<glm::vec4>(8, color);
	mesh2.setColors(vcolors);
	std::vector<GLuint> trianles(numDiv * 3);
	for (int i = 0; i < 6; i++) {
		trianles[i * 3 + 0] = 0;
		trianles[i * 3 + 1] = i + 1;
		trianles[i * 3 + 2] = i + 2;
	}
	mesh2.setTriangles(trianles);

	addCircle(obj, {0.0f, 0.09f, 0.0f}, 0.3, color);
	glm::vec4 white = {1.0f, 1.0f, 1.0f, 1.0f};
	obj->addComponent<LineRenderer>()
		->setPositions({{0.1f, 0.25f, 0.0f}, {-0.1f, 0.2f, 0.0f}})
		->setColors({white, white})
		->setWidth(3);
	obj->addComponent<LineRenderer>()
		->setPositions({{-0.1f, 0.0f, 0.0f}, {-0.1f, 0.15f, 0.0f}, {0.1f, 0.15f, 0.0f}, {0.1f, 0.0f, 0.0f}})
		->setColors({white, white, white, white})
		->setWidth(3);
	obj->addComponent<LineRenderer>()
		->setPositions({{0.0f, 0.225f, 0.0f}, {0.0f, -0.1f, 0.0f}})
		->setColors({white, white})
		->setWidth(3);

	addFivePointedStar(obj, {0.67f, 0.09f, 0.0f}, 0.13, color);

	return obj;
}

void MyApplication::addCircle(qy::cg::DisplayObject* obj, glm::vec3 center, float r, glm::vec4 color) {
	std::vector<glm::vec3> vertices(numDiv + 1);
	vertices[0] = center;
	std::vector<glm::vec4> vcolors(numDiv + 1);
	vcolors[0] = color;
	std::vector<GLuint> trianles(numDiv * 3);

	using namespace std::numbers;
	for (int i = 0; i < numDiv; i++) {
		vertices[i + 1] = center + glm::vec3(r * cos(pi * 2 * i / numDiv), r * sin(pi * 2 * i / numDiv), 0);
		vcolors[i + 1] = color;
		trianles[i * 3 + 0] = 0;
		trianles[i * 3 + 1] = i % numDiv + 1;
		trianles[i * 3 + 2] = (i + 1) % numDiv + 1;
	}
	using namespace qy::cg;
	auto meshRender = obj->addComponent<MeshRenderer>();
	auto&& mesh = meshRender->getMesh();
	mesh.setVertices(vertices);
	mesh.setColors(vcolors);
	mesh.setTriangles(trianles);
}

void MyApplication::addFivePointedStar(qy::cg::DisplayObject* obj, glm::vec3 center, float r1, glm::vec4 color, float theta, float r2)
{
	// r1,r2分别为内外顶点半径, theta为偏转角度(角度制)
	using namespace std::numbers;
	if (0 == r2) r2 = r1 / sin(18.0 / 180 * pi) * sin(126.0 / 180 * pi); //正五角

	std::vector<glm::vec3> vertices(5 * 2 + 1);
	vertices[0] = center;
	std::vector<glm::vec4> vcolors(5 * 2 + 1);
	vcolors[0] = color;
	std::vector<GLuint> trianles(5 * 2 * 3);
	using namespace std::numbers;
	for (int i = 0; i < 5; i++) {
		vertices[i * 2 + 1] = center + glm::vec3(r2 * cos((18 + 72 * i + theta) / 180.0 * pi), r2 * sin((18 + 72 * i + theta) / 180.0 * pi), 0);
		vertices[i * 2 + 2] = center + glm::vec3(r1 * cos((54 + 72 * i + theta) / 180.0 * pi), r1 * sin((54 + 72 * i + theta) / 180.0 * pi), 0);
		vcolors[i * 2 + 1] = color;
		vcolors[i * 2 + 2] = color;
		trianles[i * 6 + 0] = 0;
		trianles[i * 6 + 1] = i * 2 % 10 + 1;
		trianles[i * 6 + 2] = (i * 2 + 1) % 10 + 1;
		trianles[i * 6 + 3] = 0;
		trianles[i * 6 + 4] = (i * 2 + 1) % 10 + 1;
		trianles[i * 6 + 5] = (i * 2 + 2) % 10 + 1;
	}
	using namespace qy::cg;
	auto meshRender = obj->addComponent<MeshRenderer>();
	auto&& mesh = meshRender->getMesh();
	mesh.setVertices(vertices);
	mesh.setColors(vcolors);
	mesh.setTriangles(trianles);
}


int main() {
	MyApplication app;
	app.createWindow(800, 800, "MyApplication");
	app.run();
	return 0;
}