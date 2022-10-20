#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {
	
	void DisplayObject::update() {
		for (auto& t : m_components) {
			if (t->enabled()) {
				t->update();
			}
		}
	}

}