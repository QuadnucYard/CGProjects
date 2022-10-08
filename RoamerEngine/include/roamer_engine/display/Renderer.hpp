#pragma once
#include "Component.hpp"

namespace qy::cg {
	class Renderer : public Component {
	public:
		DECL_OBJECT(Renderer);

		virtual void render(const glm::mat4& mat) = 0;
	};
}