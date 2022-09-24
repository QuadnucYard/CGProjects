#pragma once

#include <vector>
#include "Component.h"

namespace qy::cg {
	class DisplayObject {

	private:
		std::vector<std::shared_ptr<Component>> m_components;

	};
}