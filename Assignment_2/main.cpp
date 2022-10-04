#include <roamer_engine.h>
#include <cmath>
#include <numbers>
#include <random>

class MyApplication : public qy::cg::Application {

private:
	static constexpr int numDiv = 36;
	static constexpr float r1 = 0.4f, r2 = 0.7f;

	qy::cg::DisplayObject* obj;

protected:
	void init() override {
		//draw_Doraemon();
		draw_YiJianSanLian();
	}

	void display() override {
		glClearColor(0.5f, 0.8f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		obj->render();
	}

	void initExample()
	{
		using namespace qy::cg;
		obj = new DisplayObject();

		auto meshRenderer = obj->addComponent<MeshRenderer>();
		auto meshRenderer2 = obj->addComponent<MeshRenderer>();

		auto&& mesh = meshRenderer->getMesh();
		mesh.setVertices({ {0.0f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f} });
		mesh.setColors({ {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} });
		mesh.setTriangles({ 0, 1, 2 });
		auto&& mesh2 = meshRenderer2->getMesh();
		mesh2.setVertices({ {0.5f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f}, {1.0f, -0.5f, 0.0f} });
		mesh2.setColors({ {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 0.5f} });
		mesh2.setTriangles({ 0, 1, 2 });

		obj->addComponent<LineRenderer>()
			->setPositions({ {-0.8f, -0.8f, 0.0f}, {0.8f, 0.7f, 0.0f}, {-0.6f, 0.3f, 0.0f} })
			->setColors({ {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 0.5f} })
			->setWidth(10)
			->setLoop(true);

		std::cout << obj->getComponent<Component>() << std::endl;
		std::cout << obj->getComponent<MeshRenderer>() << std::endl;
		std::cout << obj->getComponent<LineRenderer>() << std::endl;
		std::cout << isinstance<Renderer>(meshRenderer) << std::endl;
		std::cout << isinstance<Renderer>(meshRenderer2) << std::endl;
		std::cout << obj->getComponents<MeshRenderer>().size() << std::endl;
	}

	void addCircle(glm::vec3 center, float r, glm::vec4 color);

	void addFivePointedStar(glm::vec3 center, float r1, glm::vec4 color,float theta = 0, float r2 = 0);

	void draw_Doraemon();

	void draw_YiJianSanLian();
};

void MyApplication::draw_Doraemon() {
	using namespace qy::cg;
	obj = new DisplayObject();

	glm::vec4 blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	glm::vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec4 black = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 red = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 yellow = { 1.0, 1.0f, 0.0f, 1.0f };

	addCircle({ 0.0f,0.425f,0.0f }, 0.425, blue);
	addCircle({ 0.0f,0.3f,0.0f }, 0.3, white);
	addCircle({ -0.1f, 0.55f, 0.0f }, 0.096f, black);
	addCircle({ 0.1f, 0.55f, 0.0f }, 0.096f, black);
	addCircle({ -0.1f, 0.55f, 0.0f }, 0.093f, white);
	addCircle({ 0.1f, 0.55f, 0.0f }, 0.093f, white);
	addCircle({ 0.04f, 0.55f, 0.0f }, 0.015f, black);
	addCircle({ -0.04f, 0.55f, 0.0f }, 0.015f, black);
	addCircle({ 0.0f, 0.45f, 0.0f }, 0.05f, red);
	obj->addComponent<LineRenderer>()
		->setPositions({ { 0.0f, 0.15f, 0.0f },{ 0.0f, 0.4f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({ { 0.1f, 0.35f, 0.0f },{ 0.225f, 0.4f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({ { 0.1f, 0.3f, 0.0f },{ 0.235f, 0.3f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({ { 0.1f, 0.25f, 0.0f },{ 0.215f, 0.2f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({ { -0.1f, 0.35f, 0.0f },{ -0.225f, 0.4f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({ { -0.1f, 0.3f, 0.0f },{ -0.235f, 0.3f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
	obj->addComponent<LineRenderer>()
		->setPositions({ { -0.1f, 0.25f, 0.0f },{ -0.215f, 0.2f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);

	auto&& mesh = obj->addComponent<MeshRenderer>()->getMesh();
	mesh.setVertices({ { -0.274f, 0.1f, 0.0f },{ -0.274f, 0.0f, 0.0f },{ 0.274f, 0.0f, 0.0f },{ 0.274f, 0.1f, 0.0f } });
	mesh.setColors({ red, red, red,red });
	mesh.setTriangles({ 0, 1, 2, 0, 2, 3 });

	addCircle({ 0.0f, -0.025f, 0.0f }, 0.0725f, black);
	addCircle({ 0.0f, -0.025f, 0.0f }, 0.07f, yellow);

	obj->addComponent<LineRenderer>()
		->setPositions({ { -0.072f, -0.025f, 0.0f },{ 0.072f, -0.025f, 0.0f } })
		->setColors({ black, black })
		->setWidth(1);
}

void MyApplication::draw_YiJianSanLian() {
	using namespace qy::cg;
	obj = new DisplayObject();

	auto meshRenderer = obj->addComponent<MeshRenderer>();
	auto&& mesh = meshRenderer->getMesh();
	mesh.setVertices({ { -0.925f, 0.2f, 0.0f },{ -0.925f, -0.2f, 0.0f },{ -0.98f, 0.18f, 0.0f },{ -0.98f, -0.18f, 0.0f } });
	glm::vec4 color = { 0.0f, 161.0f / 255, 214.0f / 255, 1.0f };
	std::vector<glm::vec4> vcolors(4, color);
	mesh.setColors(vcolors);
	mesh.setTriangles({ 0, 2, 3, 0, 3, 1 });

	auto meshRenderer2 = obj->addComponent<MeshRenderer>();
	auto&& mesh2 = meshRenderer2->getMesh();
	mesh2.setVertices({
		{ -0.65f, 0.2f, 0.0f },
		{ -0.35f, 0.2f, 0.0f },
		{ -0.45f, -0.2f, 0.0f },
		{ -0.9f, -0.2f, 0.0f },
		{ -0.9f, 0.2f, 0.0f },
		{ -0.8f, 0.3f, 0.0f },
		{ -0.75f, 0.45f, 0.0f },
		{ -0.65f, 0.45f, 0.0f },
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

	addCircle({ 0.0f, 0.09f, 0.0f }, 0.3, color);
	glm::vec4 white = { 1.0f, 1.0f, 1.0f, 1.0f };
	obj->addComponent<LineRenderer>()
		->setPositions({ { 0.1f, 0.25f, 0.0f },{ -0.1f, 0.2f, 0.0f } })
		->setColors({ white, white })
		->setWidth(3);
	obj->addComponent<LineRenderer>()
		->setPositions({ { -0.1f, 0.0f, 0.0f }, { -0.1f, 0.15f, 0.0f }, {0.1f, 0.15f, 0.0f}, {0.1f, 0.0f, 0.0f} })
		->setColors({ white, white, white, white })
		->setWidth(3);
	obj->addComponent<LineRenderer>()
		->setPositions({ { 0.0f, 0.225f, 0.0f },{ 0.0f, -0.1f, 0.0f } })
		->setColors({ white, white })
		->setWidth(3);

	addFivePointedStar({ 0.67f, 0.09f, 0.0f }, 0.13, color);
}

void MyApplication::addCircle(glm::vec3 center, float r, glm::vec4 color) {
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

void MyApplication::addFivePointedStar(glm::vec3 center, float r1, glm::vec4 color, float theta, float r2)
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