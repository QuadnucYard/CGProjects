#pragma once
#include "Component.hpp"

namespace qy::cg {

	class Mesh;

	class MeshFilter: public Component {
		
	public:
		DECL_OBJECT(MeshFilter);

		ptr<Mesh> mesh();
		ptr<Mesh> sharedMesh();
		void setMesh(const ptr<Mesh>& mesh);
		void setSharedMesh(const ptr<Mesh>& mesh);

	private:
		DECL_PIMPL;
	};
}