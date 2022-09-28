#pragma once
#include "Component.h"

namespace qy::cg {
	class Renderer : public Component {
	public:
		virtual void render() = 0;
	};
}