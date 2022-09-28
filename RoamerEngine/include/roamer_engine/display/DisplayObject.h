#pragma once

#include <vector>
#include "Renderer.h"

namespace qy::cg {
	class DisplayObject : public Object {

	private:
		std::vector<std::shared_ptr<Component>> m_components;

	public:
		template <class T>
		std::shared_ptr<T> addComponent() {
			auto comp = std::make_shared<T>();
			m_components.push_back(std::dynamic_pointer_cast<Component>(comp));
			return comp;
		}

		template <class T>
		std::shared_ptr<T> getComponent() {
			auto it = std::find_if(m_components.begin(), m_components.end(),
				[](auto& t) { return isinstance<T>(t); }
			);
			if (it == m_components.end()) return nullptr;
			return std::dynamic_pointer_cast<T>(*it);
		}

		template <class T>
		std::vector<std::shared_ptr<T>> getComponents() {
			std::vector<std::shared_ptr<T>> ret;
			for (auto& t : m_components) {
				if (isinstance<T>(t)) ret.push_back(std::dynamic_pointer_cast<T>(t));
			}
			return ret;
		}

		void render() {
			for (auto& t : m_components) {
				if (isinstance<Renderer>(t)) {
					std::dynamic_pointer_cast<Renderer>(t)->render();
				}
			}
		}
	};
}