#include "roamer_engine/display/Component.hpp"
#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {
	
	struct Component::Impl {
		std::weak_ptr<DisplayObject> obj;
	};

	Component::Component() : pImpl(std::make_unique<Component::Impl>()) {}
	Component::~Component() = default;

	std::shared_ptr<DisplayObject> Component::obj() { return pImpl->obj.lock(); }

	void Component::_setObj(std::shared_ptr<DisplayObject> _obj) {
		pImpl->obj = _obj;
	}
}