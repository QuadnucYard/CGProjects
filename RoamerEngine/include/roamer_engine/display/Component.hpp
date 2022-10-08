#pragma once
#include "../Object.hpp"
#include <vector>

namespace qy::cg {

	class DisplayObject;
	class Component;

	template< typename T>
	concept ComponentType = std::is_base_of_v<Component, T>;

	class Component: public Object, public std::enable_shared_from_this<Component> {
	public:
		DECL_OBJECT(Component);

		virtual void start() {}
		virtual void update() {}

		std::shared_ptr<DisplayObject> obj();

		bool enabled() const;
		void enabled(bool value);

		template <ComponentType T>
		std::shared_ptr<T> addComponent();

		template <ComponentType T>
		std::shared_ptr<T> getComponent();

		template <ComponentType T>
		std::vector<std::shared_ptr<T>> getComponents();

	private:
		void _setObj(std::shared_ptr<DisplayObject> _obj);

	private:
		DECL_PIMPL;
		
		friend class DisplayObject;
	};

	template <ComponentType T>
	std::shared_ptr<T> Component::addComponent() { return obj()->template addComponent<T>(); }

	template <ComponentType T>
	std::shared_ptr<T> Component::getComponent() { return obj()->template getComponent<T>(); }

	template <ComponentType T>
	std::vector<std::shared_ptr<T>> Component::getComponents() { return obj()->template getComponents<T>(); }

}