#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {
	
	void DisplayObject::update() {
		for (auto& t : m_components) {
			if (t->enabled()) {
				t->update();
			}
		}
	}

	void DisplayObject::render() {
		for (auto& t : m_components) {
			if (t->enabled() && isinstance<Renderer>(t)) {
				std::dynamic_pointer_cast<Renderer>(t)->render(transform()->localToWorldMatrix());
			}
		}
	}
}