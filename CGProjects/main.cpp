#include <roamer_engine.h>
#include <cmath>
#include <numbers>
#include <random>

class MyApplication: public qy::cg::Application {

private:
	static constexpr int numDiv = 36;
	static constexpr float r1 = 0.4f, r2 = 0.7f;

	qy::cg::DisplayObject* obj;

protected:
	void init() override {
		using namespace qy::cg;

		obj = new DisplayObject();

		auto meshRenderer = obj->addComponent<MeshRenderer>();
		auto meshRenderer2 = obj->addComponent<MeshRenderer>();

		auto&& mesh = meshRenderer->getMesh();
		mesh.setVertices({{0.0f, 0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}});
		mesh.setColors({{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}});
		mesh.setTriangles({0, 1, 2});
		auto&& mesh2 = meshRenderer2->getMesh();
		mesh2.setVertices({{0.5f, 0.5f, 0.0f}, {0.0f, -0.5f, 0.0f}, {1.0f, -0.5f, 0.0f}});
		mesh2.setColors({{1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 0.5f}});
		mesh2.setTriangles({0, 1, 2});

		obj->addComponent<LineRenderer>()
			->setPositions({{-0.8f, -0.8f, 0.0f}, {0.8f, 0.7f, 0.0f}, {-0.6f, 0.3f, 0.0f}})
			->setColors({{1.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 0.5f}})
			->setWidth(10)
			->setLoop(true);

		std::cout << obj->getComponent<Component>() << std::endl;
		std::cout << obj->getComponent<MeshRenderer>() << std::endl;
		std::cout << obj->getComponent<LineRenderer>() << std::endl;
		std::cout << isinstance<Renderer>(meshRenderer) << std::endl;
		std::cout << isinstance<Renderer>(meshRenderer2) << std::endl;
		std::cout << obj->getComponents<MeshRenderer>().size() << std::endl;
	}

	void update() override {
		using namespace qy::cg;
		printf("%u: %lf %lf\n", Time::frameCount(), Time::deltaTime(), Time::time());
	}

	void display() override {
		glClearColor(0.5f, 0.8f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		obj->render();
	}
};

int main() {
	MyApplication app;
	app.createWindow(800, 800, "MyApplication");
	app.run();
	return 0;
}