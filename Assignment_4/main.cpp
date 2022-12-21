#include <roamer_engine.hpp>
#include <cmath>
#include <numbers>
#include <random>
#include <roamer_editor.hpp>
#include "Maze.hpp"
#include "Wall.hpp"

using namespace qy::cg;

auto loadModel(std::string_view obj, std::string_view tex) {
	auto o = ModelLoader::loadObj(fs::path("assets") / obj)->transform();
	o->position({2, 0, 0});
	o->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Assets::load<Texture2D>(fs::path("assets") / tex));
	return o;
}

class Spinning : public Component {
public:
	void update() override {
		transform()->rotation(glm::vec3 {0.0, Time::time() * 10, 0.0});
	}
};

class Flicker : public Component {
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
		auto hue = Random::range(0.0f, 1.0f);
		auto color = Color::hsv2rgb(hue, 1.0f, 0.4f, 1.0);
		auto lightColor = Color::hsv2rgb(hue, 1.0f, 1.0f, 1.0);
		obj->getComponent<MeshRenderer>()->getMaterial()->setShader(Shaders::Unlit);
		auto&& light = obj->addComponent<Light>();
		light->setType(LightType::Spot);
		light->setAmbient({0.0f, 0.0f, 0.0f, 1.0f});
		light->setDiffuse(color);
		light->setSpecular(color);
		light->setIntensity(1.0f);
		light->setRange(40);

		obj->getComponent<MeshRenderer>()->getMaterial()->setColor(lightColor);
		obj->addComponent<Flicker>();
	}
	ptr<DisplayObject> getObj() { return obj; }
};

class roundPointLight {
private:
	ptr<DisplayObject> obj;
	float shootSpeed = 0.01;
	vec3 deltaPos = {-0.02, 0.01, -0.04};

	struct Point {
		ptr<DisplayObject> pointLight;
		float r, theta, height, v, offset;
	};

	std::vector<Point> points;

public:
	roundPointLight() {
		obj = DisplayObject::create();

		for (int i = 0; i < 10;i++) {
			const float pi = std::numbers::pi_v<float>;
			auto r = Random::range(0.01f, 0.015f);
			auto theta = Random::range(0.0f, 2 * pi);
			auto height = Random::range(-0.005f,  0.005f);
			auto v = Random::range(2.0f, 3.0f);
			auto offest = Random::range(1.0f);

			auto pointsObject = Primitives::createSphere();
			pointsObject->transform()->scale({0.001, 0.001, 0.001});
			pointsObject->transform()->position(deltaPos + glm::vec3{r * cos(theta), r * sin(theta), height});
			auto color = glm::vec4 {1.0, 1.0, 1.0, 1.0};
			pointsObject->getComponent<MeshRenderer>()->getMaterial()->setShader(Shaders::Unlit);
			pointsObject->getComponent<MeshRenderer>()->getMaterial()->setColor(color);
			auto&& light = pointsObject->addComponent<Light>();
			light->setType(LightType::Point);
			light->setAmbient({0.0f, 0.0f, 0.0f, 1.0f});
			light->setDiffuse(color);
			light->setSpecular(color);
			light->setIntensity(0.01f);

			obj->transform()->addChild(pointsObject->transform());
			points.push_back(Point{pointsObject, r, theta, height, v, offest});
		}
	}

	ptr<DisplayObject> getObj() { return obj; }

	void update() {
		auto t = 0.3 * Time::time();
		for (auto p : points) {
			auto r = p.r + 0.01 * sin(t + p.offset);
			auto theta = p.theta + p.v * t;
			const float pi = std::numbers::pi_v<float>;
			if (theta > 2 * pi) theta -= 2 * pi;
			auto h = p.height + 0.001 * sin(t + p.offset);
			p.pointLight->transform()->position(deltaPos + glm::vec3{r * cos(theta), r * sin(theta), h});
		}
	}
};

//线性插值
glm::quat Nlerp(glm::quat q1, glm::quat q2, float t) {
	return ((1 - t) * q1 + t * q2) * (1.0f / glm::length(((1 - t) * q1 + t * q2)));
}

//球面插值
glm::quat Slerp(glm::quat q1, glm::quat q2, float t) {
	auto theta = glm::acos(glm::dot(q1, q2));
	if (sin(theta) < 0.001 && sin(theta) > -0.001) return Nlerp(q1, q2, t);
	return q1 * sin((1.0f - t) * theta) / sin(theta) + q2 * sin(t * theta) / sin(theta);
}

class MyApplication : public RoamerEditor::EditorApplication {
private:
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Camera> cam;
	ptr<DisplayObject> obj, monkey;
	ptr<roundPointLight> roundPoints;
protected:
	void init() override {
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);*/
		scene = Scene::create();
		auto root = scene->root();
		cam = scene->createCamera();
		cam->setFarClipPlane(100);
		//cam->setBackgroundColor({ 0.4f, 0.3f, 0.1f, 1.0f });
		cam->obj()->transform()->position({0, 0, 0});
		cam->obj()->transform()->rotation(glm::vec3({0.0, glm::radians(-90.0), 0.0}));
		cam->addComponent(SkyBox::loadFromFile(
			"assets/skybox/interstellar_rt.png",
			"assets/skybox/interstellar_lf.png",
			"assets/skybox/interstellar_up.png",
			"assets/skybox/interstellar_dn.png",
			"assets/skybox/interstellar_ft.png",
			"assets/skybox/interstellar_bk.png"
		));
		cam->setClearFlags(CameraClearFlags::Skybox);
		cam->addComponent<MoveController>()->setMoveType(MoveType::Free);

		//auto&& light = cam->addComponent<Light>();
		//light->setType(LightType::Spot);
		//light->setAmbient({0.1f, 0.1f, 0.1f, 1.0f});
		//light->setDiffuse(Color::rgba(250, 250, 236));
		//light->setSpecular(Color::rgba(250, 250, 236));
		//light->setIntensity(0.5f);
		//light->setRange(20);
		//light->setInnerSpotAngle(20);
		//light->setSpotAngle(60);
		//light->setShadows(LightShadow::Soft);

		cam->setNearClipPlane(0.01f);
		cam->setFieldOfView(160.0f);

		int width = 7;
		int height = 7;
		Maze m(width, height);
		auto maze = m.getMaze();
		auto mazeContainer = DisplayObject::create()->transform();
		mazeContainer->obj()->name("Maze container");
		scene->root()->addChild(mazeContainer);
		mazeContainer->position({-6.7, -2.5, 2.5});
		for (int i = 1; i <= width; i++) {
			for (int j = 1; j <= height; j++) {
				if ((i == 1 && j == height - 1)) {
					Wall w1, w2;
					w1.position({i * 2.0, 2.0, (j - height + 1) * 2.0});
					mazeContainer->addChild(w1.getObj()->transform());
					w2.position({i * 2.0, -2.0, (j - height + 1) * 2.0});
					mazeContainer->addChild(w2.getObj()->transform());
					if (i % 2 == 0 && j % 2 == 0) {
						TopLight light;
						light.getObj()->transform()->position({i * 2.0, 1.0, (j - height + 1) * 2.0});
						mazeContainer->addChild(light.getObj()->transform());
					}
					continue;
				}

				if (maze[i][j] == -1) {
					Wall w;
					w.position({i * 2.0, 0.0, (j - height + 1) * 2.0});
					mazeContainer->addChild(w.getObj()->transform());
				} else {
					Wall w1, w2;
					w1.position({i * 2.0, 2.0, (j - height + 1) * 2.0});
					mazeContainer->addChild(w1.getObj()->transform());
					w2.position({i * 2.0, -2.0, (j - height + 1) * 2.0});
					mazeContainer->addChild(w2.getObj()->transform());
					if (i % 2 == 0 && j % 2 == 0) {
						TopLight light;
						light.getObj()->transform()->position({i * 2.0, 1.0, (j - height + 1) * 2.0});
						mazeContainer->addChild(light.getObj()->transform());
					}
				}
			}
		}

		auto container = DisplayObject::create("Container")->transform();
		vec3 offset {6.7, 2.5, -2.5};
		//container->position({6.7, 2.5, -2.5});
		//scene->root()->addChild(container);
		{
			auto lightObj = Primitives::createSphere();
			lightObj->name("Light1");
			lightObj->transform()->scale({0.1f, 0.1f, 0.1f});
			lightObj->transform()->rotation(glm::radians(vec3 {-90, 0, 0}));
			container->addChild(lightObj->transform());
			lightObj->transform()->position(vec3 {1, 4, 1} + offset);
			auto&& light = lightObj->addComponent<Light>();
			light->setType(LightType::Spot);
			light->setIntensity(0.5f);
			light->setAmbient({0.5f, 0.0f, 0.0f, 1.0f});
			light->setDiffuse({1.0f, 0.1f, 0.1f, 1.0f});
			light->setSpecular({1.0f, 1.0f, 1.0f, 1.0f});
			light->setRange(1000);
			light->setShadows(LightShadow::Soft);
		}
		{
			auto lightObj = Primitives::createSphere();
			lightObj->name("Light2");
			lightObj->transform()->scale({0.1f, 0.1f, 0.1f});
			container->addChild(lightObj->transform());
			lightObj->transform()->position(vec3 {-1, 3, 1} + offset);
			auto&& light = lightObj->addComponent<Light>();
			light->setType(LightType::Point);
			light->setIntensity(0.5f);
			light->setAmbient({0.0f, 0.5f, 0.0f, 1.0f});
			light->setDiffuse({0.1f, 1.0f, 0.1f, 1.0f});
			light->setSpecular({1.0f, 1.0f, 1.0f, 1.0f});
			light->setRange(1000);
			light->setShadows(LightShadow::Hard);
		}
		{
			auto lightObj = Primitives::createSphere();
			lightObj->name("Light3");
			lightObj->transform()->scale({0.1f, 0.1f, 0.1f});
			container->addChild(lightObj->transform());
			lightObj->transform()->position(vec3 {0, 2, -1} + offset);
			auto&& light = lightObj->addComponent<Light>();
			light->setType(LightType::Point);
			light->setIntensity(0.5f);
			light->setAmbient({0.0f, 0.0f, 0.5f, 1.0f});
			light->setDiffuse({0.1f, 0.1f, 1.0f, 1.0f});
			light->setSpecular({1.0f, 1.0f, 1.0f, 1.0f});
			light->setRange(1000);
			light->setShadows(LightShadow::Soft);
		}
		scene->setAmbientColor({0.2f, 0.2f, 0.2f, 1.0f});
		{
			auto obj = Primitives::createCube();
			obj->name("Ground");
			obj->transform()->position({0, -7, 0});
			obj->transform()->scale({100, 1, 100});
			obj->getComponent<MeshRenderer>()->getMaterial()->setMainTexture(Assets::load<Texture2D>("assets/Rock050_1K_Color.jpg"));
			//obj->getComponent<MeshRenderer>()->getMaterial()->setColor("material.ambient", {0.1f, 0.1f, 0.1f, 1.0f});
			scene->root()->addChild(obj->transform());
		}
		{
			auto&& obj2 = loadModel("ApexPlasmaMasterGeo.obj", "ApexPlasmaMasterDiffuse.png");
			obj2->transform()->scale({0.05f, 0.05f, 0.05f});
			container->addChild(obj2->transform());
			obj2->name("ApexPlasmaMaster");
		}
		{
			auto chicken = loadModel("ChickenGeo.obj", "ApexPlasmaMasterDiffuse.png")->transform();
			chicken->position({2, 0, 0});
			root->addChild(chicken);
			chicken->obj()->name("Chicken");
		}
		{
			auto sungod = loadModel("TrueSunGod.obj", "TrueSunGod.png")->transform();
			sungod->position({-2, 0, 0});
			sungod->scale({0.1, 0.1, 0.1});
			root->addChild(sungod);
			sungod->obj()->name("TrueSunGod");
		}

		{
			roundPoints = std::make_shared<roundPointLight>();
			root->addChild(roundPoints->getObj()->transform());
		}
	}

	void update() override {
		using namespace qy::cg;
		auto&& transform = roundPoints->getObj()->transform();
		auto dp = cam->transform()->position() - transform->position();
		transform->position(transform->position() + dp * 0.1f);
		transform->rotation(Nlerp(transform->rotation(), cam->transform()->rotation(), 0.1));
		roundPoints->update();

		if (cam->getComponent<Light>()) {
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
		}
		scene->dispatch_update();
		if (Input::getKeyDown(KeyCode::F11)) {
			Screen::setFullScreen(!Screen::isFullScreen());
		}
		if (Input::getKeyDown(KeyCode::ESCAPE)) {
			quit();
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
