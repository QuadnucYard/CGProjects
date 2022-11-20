#include "roamer_engine/display/MeshRenderer.hpp"
#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/Mesh.hpp"
#include "roamer_engine/display/DisplayObject.hpp"

namespace qy::cg {

	struct MeshRenderer::Impl {
		
	};

	DEFINE_OBJECT(MeshRenderer);

	void MeshRenderer::__render() {
		getComponent<MeshFilter>()->mesh()->__render();
	}

}