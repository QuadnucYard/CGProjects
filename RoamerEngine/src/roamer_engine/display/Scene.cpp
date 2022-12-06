#include "roamer_engine/display/Scene.hpp"
#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Transform.hpp"

namespace qy::cg {

	struct Scene::Impl {
		TransformPtr root;
		color_t ambientColor {0.0f, 0.0f, 0.0f, 0.0f};
	};

	DEFINE_OBJECT(Scene);

	ptr<Scene> Scene::create() {
		auto scene = instantiate<Scene>();
		scene->name("Scene");
		scene->pImpl->root = instantiate<Transform>();
		_currentScene = scene;
		return scene;
	}

	ptr<Scene> Scene::current() {
		return _currentScene;
	}

	ptr<Transform> Scene::root() {
		return pImpl->root;
	}

	ptr<Camera> Scene::createCamera() {
		auto obj = DisplayObject::create();
		auto cam = obj->addComponent<Camera>();
		root()->addChild(obj->transform());
		return cam;
	}

	void Scene::dispatch_update() {
		for (auto&& c : *pImpl->root) {
			if (c->obj()->activeSelf())
				c->obj()->update();
		}
	}

	void Scene::dispatch_render() {
		if (Camera::main())
			Camera::main()->render();
	}

	color_t Scene::getAmbientColor() const { return pImpl->ambientColor; }
	void Scene::setAmbientColor(color_t value) { pImpl->ambientColor = value; }
}