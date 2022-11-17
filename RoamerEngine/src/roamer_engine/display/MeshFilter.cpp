#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/Mesh.hpp"

namespace qy::cg {

	struct MeshFilter::Impl {
		ptr<Mesh> mesh;
		ptr<Mesh> sharedMesh;
	};

	DEFINE_OBJECT(MeshFilter);

	ptr<Mesh> MeshFilter::mesh() {
		if (!pImpl->mesh) pImpl->mesh = sharedMesh(); return pImpl->mesh;
	}

	ptr<Mesh> MeshFilter::sharedMesh() {
		if (!pImpl->sharedMesh) pImpl->sharedMesh = instantiate<Mesh>();
		return pImpl->sharedMesh;
	}

	void MeshFilter::setMesh(const ptr<Mesh>& mesh) {
		pImpl->mesh = mesh;
	}

	void MeshFilter::setSharedMesh(const ptr<Mesh>& mesh) { 
		pImpl->mesh = pImpl->sharedMesh = mesh; 
	}

}