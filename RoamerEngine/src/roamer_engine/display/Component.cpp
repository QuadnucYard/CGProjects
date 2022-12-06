#include "roamer_engine/display/Component.hpp"
#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {

	struct Component::Impl {
		ptr<DisplayObject> obj;
		bool enabled {true};
	};

	DEFINE_OBJECT(Component);

	ptr<DisplayObject> Component::obj() const { return pImpl->obj; }

	bool Component::enabled() const { return pImpl->enabled; }
	void Component::enabled(bool value) { pImpl->enabled = value; }

	ptr<Transform> Component::transform() const { return pImpl->obj->transform(); }

	const ptr_vector<Component>& Component::getComponents() const { return pImpl->obj->getComponents(); }

	void Component::_setObj(ptr<DisplayObject> _obj) {
		pImpl->obj = _obj;
	}
}