#pragma once
#include "DisplayObject.hpp"
#include "Camera.hpp"

namespace qy::cg {

	class Scene : public Object {
	private:
		inline static ptr<Scene> _currentScene;

	public:
		Scene() {}

		static ptr<Scene> create() { 
			auto scene = instantiate<Scene>(); 
			scene->m_root = instantiate<Transform>();
			_currentScene = scene;
			return scene;
		}

		static ptr<Scene> current() {
			return _currentScene;
		}

		auto root() { 
			return m_root;
		}

		auto createCamera() {
			auto obj = DisplayObject::create();
			auto cam = obj->addComponent<Camera>();
			root()->addChild(obj->transform());
			return cam;
		}

		void dispatch_update() {
			for (auto&& c : *m_root) {
				c->obj()->update();
			}
		}

		void dispatch_render() {
			if (Camera::main())
				Camera::main()->render();
		}

	private:
		TransformPtr m_root;
	};
}