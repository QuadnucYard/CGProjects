#pragma once
#include "Component.hpp"
#include <vector>

namespace qy::cg {

	class Transform;

	class DisplayObject final: public Object, public std::enable_shared_from_this<DisplayObject> {

	public:
		DECL_OBJECT(DisplayObject);

		void _init();

		static ptr<DisplayObject> create();
		static ptr<DisplayObject> create(std::string name);

		bool activeSelf() const;
		bool activeInHierarchy() const;
		void setActive(bool value);

		ptr<Transform> transform() const;

		template <ComponentType T>
		ptr<T> addComponent();

		template <ComponentType T>
		ptr<T> addComponent(const ptr<T>& component);

		template <ComponentType T>
		ptr<T> getComponent();

		template <ComponentType T>
		ptr_vector<T> getComponents();

		const ptr_vector<Component>& getComponents() const;
		ptr_vector<Component>& getComponents();

		virtual void update();

	private:
		DECL_PIMPL;
	};


	template <ComponentType T>
	inline ptr<T> DisplayObject::addComponent() {
		return addComponent(instantiate<T>());
	}

	template<ComponentType T>
	inline ptr<T> DisplayObject::addComponent(const ptr<T>& comp) {
		auto baseComp = std::dynamic_pointer_cast<Component>(comp);
		getComponents().push_back(baseComp);
		baseComp->_setObj(shared_from_this());
		comp->start();
		return comp;
	}

	template<ComponentType T>
	inline ptr<T> DisplayObject::getComponent() {
		auto&& components = getComponents();
		auto it = std::ranges::find_if(components, [](auto& t) { return isinstance<T>(t); });
		if (it == components.end()) return nullptr;
		return std::dynamic_pointer_cast<T>(*it);
	}

	template<ComponentType T>
	inline ptr_vector<T> DisplayObject::getComponents() {
		ptr_vector<T> ret;
		for (auto& t : getComponents()) {
			if (isinstance<T>(t)) ret.push_back(std::dynamic_pointer_cast<T>(t));
		}
		return ret;
	}

}