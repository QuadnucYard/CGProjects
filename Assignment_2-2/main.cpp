#include <roamer_engine.hpp>
#include <cmath>
#include <numbers>

class SimpleParticle: public qy::cg::Component {

private:
	float spawnTime;
	bool dead;
	float speed;
	float rotSpeed;

public:
	void init() {
		using namespace qy::cg;
		addCircle(
			Random::range(0.01f, 0.2f),
			Color::hsv2rgb(Random::range(0.0f, 1.0f), 1.0f, 0.5f, Random::range(0.0f, 1.0f)),
			Random::range(3, 13)
		);
		spawnTime = Random::range(0.0f, 5.0f);
		dead = false;
		speed = Random::range(1.0f, 5.0f);
		rotSpeed = std::sqrt(Random::range(0.3f, 8.0f));
		dead = true;
	}

	void update() {
		using namespace qy::cg;
		float dt = Time::time() - spawnTime;
		if (dt < 0) return;
		if (dead) getComponent<MeshRenderer>()->enabled(true);
		dead = false;
		float s = std::sin(dt * speed);
		obj()->transform()->rotation(glm::vec3 {0.0f, 0.0f, dt * rotSpeed});
		obj()->transform()->scale({s, s, s});
		//if (s < 0) dead = true;
	}

private:
	void addCircle(float r, glm::vec4 color, int numDiv = 24) {
		std::vector<glm::vec3> vertices(numDiv + 1);
		vertices[numDiv] = {0, 0, 0};
		std::vector<glm::vec4> vcolors(numDiv + 1, color);
		std::vector<GLuint> trianles(numDiv * 3);

		using namespace std::numbers;
		for (int i = 0; i < numDiv; i++) {
			vertices[i] = glm::vec3(r * cos(pi * 2 * i / numDiv), r * sin(pi * 2 * i / numDiv), 0);
			vcolors[i + 1] = color;
			trianles[i * 3 + 0] = numDiv;
			trianles[i * 3 + 1] = i % numDiv;
			trianles[i * 3 + 2] = (i + 1) % numDiv;
		}
		using namespace qy::cg;
		auto meshRender = obj()->addComponent<MeshRenderer>();
		auto&& mesh = meshRender->getMesh();
		mesh.setVertices(vertices);
		mesh.setColors(vcolors);
		mesh.setTriangles(trianles);
		meshRender->enabled(false);
	}
};

class MyApplication: public qy::cg::Application {

private:
	std::vector<std::shared_ptr<qy::cg::DisplayObject>> objs;
	std::shared_ptr<qy::cg::Scene> scene;

protected:
	void init() override {
		using namespace qy::cg;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		scene = Scene::create();
		auto cam = scene->createCamera();
		cam->setBackgroundColor({0.4f, 0.3f, 0.1f, 1.0f});
		cam->transform()->position({0, 0, 1.0f});
		for (int i = 0; i < 256; i++) {
			auto obj = DisplayObject::create();
			obj->addComponent<SimpleParticle>()->init();
			obj->transform()->position({Random::range(-0.9f, 0.9f), Random::range(-0.9f, 0.9f), 0});
			objs.push_back(obj);
			scene->root()->addChild(obj->transform());
		}
	}

	void update() override {
		scene->dispatch_update();
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