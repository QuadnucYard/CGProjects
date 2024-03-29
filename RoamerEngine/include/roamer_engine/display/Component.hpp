﻿#pragma once
#include "../Object.hpp"
#include <vector>

namespace qy::cg {

	class DisplayObject;
	class Component;
	class Transform;

	template< typename T>
	concept ComponentType = std::is_base_of_v<Component, T>;

	class Component: public Object, public std::enable_shared_from_this<Component> {
	public:
		DECL_OBJECT(Component);

		virtual void start() {}
		virtual void update() {}

		ptr<DisplayObject> obj() const;

		bool enabled() const;
		void enabled(bool value);

		ptr<Transform> transform() const;

		template <ComponentType T>
		ptr<T> addComponent(const ptr<T>& component);

		template <ComponentType T>
		ptr<T> addComponent();

		template <ComponentType T>
		ptr<T> getComponent() const;

		template <ComponentType T>
		ptr_vector<T> getComponents() const;

		const ptr_vector<Component>& getComponents() const;

	private:
		void _setObj(ptr<DisplayObject> _obj);

	private:
		DECL_PIMPL;
		
		friend class DisplayObject;
	};

	template <ComponentType T>
	ptr<T> Component::addComponent() { return obj()->template addComponent<T>(); }

	template <ComponentType T>
	ptr<T> Component::addComponent(const ptr<T>& component) { return obj()->template addComponent<T>(component); }

	template <ComponentType T>
	ptr<T> Component::getComponent() const { return obj()->template getComponent<T>(); }

	template <ComponentType T>
	ptr_vector<T> Component::getComponents() const { return obj()->template getComponents<T>(); }

}