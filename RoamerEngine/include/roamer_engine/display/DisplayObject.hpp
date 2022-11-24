#pragma once

#include <vector>
#include "Renderer.hpp"
#include "Transform.hpp"

namespace qy::cg {
	class DisplayObject final : public Object, public std::enable_shared_from_this<DisplayObject> {

	private:
		std::vector<ptr<Component>> m_components;
		TransformPtr m_transform;

	public:
		DisplayObject() {}

		void _init() {
			m_transform = addComponent<Transform>();
		}

		static ptr<DisplayObject> create() {
			auto obj = instantiate<DisplayObject>();
			obj->_init();
			return obj;
		}

		TransformPtr transform() const { return m_transform; }

		template <ComponentType T>
		ptr<T> addComponent();

		template <ComponentType T>
		ptr<T> addComponent(const ptr<T>& component);

		template <ComponentType T>
		ptr<T> getComponent();

		template <ComponentType T>
		std::vector<ptr<T>> getComponents();

		std::vector<ptr<Component>> getComponents() {
			return m_components;
		}

		virtual void update();
	};


	template <ComponentType T>
	ptr<T> DisplayObject::addComponent() {
		return addComponent(instantiate<T>());
	}

	template<ComponentType T>
	inline ptr<T> DisplayObject::addComponent(const ptr<T>& comp) {
		auto baseComp = std::dynamic_pointer_cast<Component>(comp);
		m_components.push_back(baseComp);
		baseComp->_setObj(shared_from_this());
		comp->start();
		return comp;
	}

	template<ComponentType T>
	inline ptr<T> DisplayObject::getComponent() {
		auto it = std::find_if(m_components.begin(), m_components.end(),
			[](auto& t) { return isinstance<T>(t); }
		);
		if (it == m_components.end()) return nullptr;
		return std::dynamic_pointer_cast<T>(*it);
	}

	template<ComponentType T>
	inline std::vector<ptr<T>> DisplayObject::getComponents() {
		std::vector<ptr<T>> ret;
		for (auto& t : m_components) {
			if (isinstance<T>(t)) ret.push_back(std::dynamic_pointer_cast<T>(t));
		}
		return ret;
	}

}