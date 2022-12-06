#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class DisplayObject;
	class Mesh;

	class Primitives final {
		using object_ptr = ptr<DisplayObject>;

	public:
		static object_ptr createUnlit(const ptr<Mesh>& mesh);
		static object_ptr createLit(const ptr<Mesh>& mesh);
		static object_ptr createCube();
		static object_ptr createSphere();
		static object_ptr createCylinder();

	};

}