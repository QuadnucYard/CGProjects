#include "roamer_engine/display/Component.hpp"
#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {

	struct Component::Impl {
		ptr<DisplayObject> obj;
		bool enabled {true};
	};

	DEFINE_OBJECT(Component);

	ptr<DisplayObject> Component::obj() { return pImpl->obj; }

	bool Component::enabled() const { return pImpl->enabled; }
	void Component::enabled(bool value) { pImpl->enabled = value; }

	ptr<Transform> Component::transform() { return pImpl->obj->transform(); }

	std::vector<ptr<Component>> Component::getComponents() { return pImpl->obj->getComponents(); }

	void Component::_setObj(ptr<DisplayObject> _obj) {
		pImpl->obj = _obj;
	}
}