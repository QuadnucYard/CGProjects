#pragma once
#include "Component.hpp"
#include "Mesh.hpp"

namespace qy::cg {

	class MeshFilter: public Component {

		ptr<Mesh> m_mesh;
	public:
		MeshFilter() : m_mesh(instantiate<Mesh>()) {}

		ptr<Mesh> mesh() { return m_mesh; }
		ptr<Mesh> sharedMesh();
	};
}