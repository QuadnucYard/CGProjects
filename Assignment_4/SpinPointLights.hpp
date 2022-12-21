#pragma once
#include <roamer_engine.hpp>

class PointSpin : public Component {
private:
	float r, theta, height, v, offset1, offset2;
	vec3 deltaPos = {-0.2, 0.1, -0.4};
	bool activate;
public:
	void start() override {
		activate = true;
		const float pi = std::numbers::pi_v<float>;
		r = Random::range(0.1f, 0.3f);
		theta = Random::range(0.0f, 2 * pi);
		height = Random::range(-0.005f, 0.005f);
		v = Random::range(2.0f, 3.0f);
		offset1 = Random::range(1.0f);
		offset2 = Random::range(1.0f);
		transform()->scale({0.01, 0.01, 0.01});
		transform()->position(deltaPos + glm::vec3 {r * cos(theta), r * sin(theta), height});
	}
	void update() override {
		if (activate) {
			auto t = 0.3 * Time::time();
			auto r_ = r + 0.01 * sin(t + offset1);
			auto theta_ = theta + v * t;
			const float pi = std::numbers::pi_v<float>;
			if (theta_ > 2 * pi) theta -= 2 * pi;
			auto height_ = height + 0.001 * sin(t + offset2);
			transform()->position(deltaPos + glm::vec3 {r_ * cos(theta_), r_ * sin(theta_), height_});
		}
	}
	void turnUP() { activate = true; }
	void turnDown() { activate = false; }
};

class ShootedPoint : public Component {
private:
	vec3 direction;
	float liveTime = 1000;
	float speed = 1;
	float shootTime;
public:
	void start() override {
		shootTime = Time::time();
	}

	void update() override {
		if (Time::time() - shootTime < liveTime)
			transform()->position(transform()->position() + speed * Time::deltaTime() * direction);
		else {
			//如何删除?
		}
	}

	void setDirection(vec3 direction_in) {
		direction = direction_in;
	}
};

using namespace qy::cg;
class SpinPointLight {

private:
	ptr<DisplayObject> obj;
	std::vector<ptr<DisplayObject>> points;
	int pointNum {10};
	
public:
	SpinPointLight() {
		obj = DisplayObject::create();

		for (int i = 0; i < pointNum; i++) {
			auto pointsObject = Primitives::createSphere();

			auto color = glm::vec4 {1.0, 1.0, 1.0, 1.0};
			pointsObject->getComponent<MeshRenderer>()->getMaterial()->setShader(Shaders::Unlit);
			pointsObject->getComponent<MeshRenderer>()->getMaterial()->setColor(color);
			auto&& light = pointsObject->addComponent<Light>();
			light->setType(LightType::Point);
			light->setAmbient({0.0f, 0.0f, 0.0f, 1.0f});
			light->setDiffuse(color);
			light->setSpecular(color);
			light->setIntensity(0.05f);

			pointsObject->addComponent<PointSpin>();
			obj->transform()->addChild(pointsObject->transform());
			points.push_back(pointsObject);
		}
	}

	ptr<DisplayObject> getObj() { return obj; }
	ptr<DisplayObject> popPoint() {
		if (!points.empty()) {
			auto p = points.back();
			points.pop_back();
			auto mat = p->transform()->localToWorldMatrix();
			auto&& transform = p->transform();
			transform->position(mat * glm::vec4 {transform->position(), 1});
			obj->transform()->removeChild(transform);
			p->getComponent<PointSpin>()->turnDown();
			return p;
		} else
			return nullptr;
	}
};