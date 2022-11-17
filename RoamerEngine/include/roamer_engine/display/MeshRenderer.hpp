#pragma once
#include "Renderer.hpp"

namespace qy::cg {
	class MeshRenderer: public Renderer {

	public:
		DECL_OBJECT(MeshRenderer);

	private:
		DECL_PIMPL;
	};
}