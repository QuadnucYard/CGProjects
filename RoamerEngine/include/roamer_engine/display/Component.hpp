#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class DisplayObject;

	class Component: public Object, public std::enable_shared_from_this<Component> {
	public:
		Component();
		~Component();

		virtual void start() {}
		virtual void update() {}

		std::shared_ptr<DisplayObject> obj();

	private:
		void _setObj(std::shared_ptr<DisplayObject> _obj);

	private:
		struct Impl;
		impl_ptr<Impl> pImpl;
		
		friend class DisplayObject;
	};

	template< typename T>
	concept ComponentType = std::is_base_of_v<Component, T>;
}