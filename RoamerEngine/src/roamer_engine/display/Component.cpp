#include "roamer_engine/display/Component.hpp"
#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {

	struct Component::Impl {
		std::shared_ptr<DisplayObject> obj;
		bool enabled {true};
	};

	DEFINE_OBJECT(Component);

	std::shared_ptr<DisplayObject> Component::obj() { return pImpl->obj; }

	bool Component::enabled() const { return pImpl->enabled; }
	void Component::enabled(bool value) { pImpl->enabled = value; }

	void Component::_setObj(std::shared_ptr<DisplayObject> _obj) {
		pImpl->obj = _obj;
	}
}