#pragma once
#include "Component.hpp"

namespace qy::cg {
	class Renderer : public Component {
	public:
		DECL_OBJECT(Renderer);

		virtual void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) = 0;
	};
}