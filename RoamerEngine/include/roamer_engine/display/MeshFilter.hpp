#pragma once
#include "Component.hpp"
#include "Mesh.hpp"

namespace qy::cg {

	class MeshFilter: public Component {

		ptr<Mesh> m_mesh;
		ptr<Mesh> m_sharedMesh;
	public:
		MeshFilter() {}

		ptr<Mesh> mesh() { if (!m_mesh) m_mesh = sharedMesh(); return m_mesh; }
		ptr<Mesh> sharedMesh() { if (!m_sharedMesh) m_sharedMesh = instantiate<Mesh>(); return m_sharedMesh; }
		void setMesh(const ptr<Mesh>& mesh) { m_mesh = mesh; }
		void setSharedMesh(const ptr<Mesh>& mesh) { m_mesh = m_sharedMesh = mesh; }
	};
}