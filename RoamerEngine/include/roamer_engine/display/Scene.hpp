#pragma once
#include "DisplayObject.hpp"
#include "Camera.hpp"

namespace qy::cg {

	class Scene : public Object {
	private:
		inline static std::shared_ptr<Scene> _currentScene;

	public:
		Scene() {}

		static std::shared_ptr<Scene> create() { 
			auto scene = instantiate<Scene>(); 
			scene->m_root = instantiate<Transform>();
			_currentScene = scene;
			return scene;
		}

		static std::shared_ptr<Scene> current() {
			return _currentScene;
		}

		auto root() { 
			return m_root;
		}

		auto createCamera() {
			auto cam = DisplayObject::create();
			cam->addComponent<Camera>();
			root()->addChild(cam->transform());
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