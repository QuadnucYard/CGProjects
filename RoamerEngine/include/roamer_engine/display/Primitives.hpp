#pragma once
#include "DisplayObject.hpp"
#include "MeshRenderer.hpp"

namespace qy::cg {

	class Primitives final {
		using object_ptr = std::shared_ptr<DisplayObject>;

	public:
		static object_ptr createCube();
		static object_ptr createSphere();

	};

}