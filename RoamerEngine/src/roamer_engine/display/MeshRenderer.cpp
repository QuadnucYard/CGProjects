#include "roamer_engine/display/MeshRenderer.h"

namespace qy::cg {
	MeshRenderer::MeshRenderer() {
		setShader(Shaders::geom_unlit);
	}

	MeshRenderer::~MeshRenderer() = default;
}