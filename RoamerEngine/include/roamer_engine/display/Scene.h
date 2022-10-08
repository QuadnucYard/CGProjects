#pragma once
#include "DisplayObject.h"

namespace qy::cg {

	class Scene : public Object {
	private:
	public:
		Scene() {  }
		static std::shared_ptr<Scene> create() { 
			auto scene = instantiate<Scene>(); 
			scene->m_root = instantiate<Transform>();
			//scene->m_root->_setObj(scene);
			return scene;
		}

		auto root() { 
			return m_root;
		}

		void dispatch_update() {
			for (auto&& c : *m_root) {
				c->obj()->update();
			}
		}

		void dispatch_render() {
			for (auto&& c : *m_root) {
				c->obj()->render();
			}
		}

	private:
		TransformPtr m_root;
	};
}