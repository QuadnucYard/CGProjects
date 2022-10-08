#pragma once

#include <vector>
#include "Renderer.h"
#include "Transform.h"

namespace qy::cg {
	class DisplayObject final : public Object, public std::enable_shared_from_this<DisplayObject> {

	private:
		std::vector<std::shared_ptr<Component>> m_components;
		TransformPtr m_transform;

	public:
		DisplayObject() {}

		void _init() {
			m_transform = addComponent<Transform>();
		}

		static std::shared_ptr<DisplayObject> create() {
			auto obj = instantiate<DisplayObject>();
			obj->_init();
			return obj;
		}

		TransformPtr transform() { return m_transform; }

		template <ComponentType T>
		std::shared_ptr<T> addComponent();

		template <ComponentType T>
		std::shared_ptr<T> getComponent() {
			auto it = std::find_if(m_components.begin(), m_components.end(),
				[](auto& t) { return isinstance<T>(t); }
			);
			if (it == m_components.end()) return nullptr;
			return std::dynamic_pointer_cast<T>(*it);
		}

		template <ComponentType T>
		std::vector<std::shared_ptr<T>> getComponents() {
			std::vector<std::shared_ptr<T>> ret;
			for (auto& t : m_components) {
				if (isinstance<T>(t)) ret.push_back(std::dynamic_pointer_cast<T>(t));
			}
			return ret;
		}

		virtual void update() {
			std::ranges::for_each(m_components, &Component::update);
		}

		virtual void render() {
			for (auto& t : m_components) {
				if (isinstance<Renderer>(t)) {
					std::dynamic_pointer_cast<Renderer>(t)->render(transform()->localToWorldMatrix());
				}
			}
		}
	};


	template <ComponentType T>
	std::shared_ptr<T> DisplayObject::addComponent() {
		auto comp = std::make_shared<T>();
		auto baseComp = std::dynamic_pointer_cast<Component>(comp);
		m_components.push_back(baseComp);
		baseComp->_setObj(shared_from_this());
		comp->start();
		return comp;
	}

}