#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class DisplayObject;

	class Component: public Object, public std::enable_shared_from_this<Component> {
	public:
		DECL_OBJECT(Component);

		virtual void start() {}
		virtual void update() {}

		std::shared_ptr<DisplayObject> obj();

	private:
		void _setObj(std::shared_ptr<DisplayObject> _obj);

	private:
		DECL_PIMPL;
		
		friend class DisplayObject;
	};

	template< typename T>
	concept ComponentType = std::is_base_of_v<Component, T>;
}