#include "roamer_engine/display/MeshRenderer.hpp"

namespace qy::cg {
	MeshRenderer::MeshRenderer() {
		setShader(Shaders::geom_unlit);
	}

	MeshRenderer::~MeshRenderer() = default;
}