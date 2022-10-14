#include "roamer_engine/display/Primitives.hpp"

namespace qy::cg {

	Primitives::object_ptr Primitives::createCube() {
		auto obj = DisplayObject::create();
		auto mr = obj->addComponent<MeshRenderer>();
		auto&& mesh = mr->getMesh();
		mesh.setVertices({
			{-1, -1, 1},
			{-1, 1, 1},
			{1, 1, 1},
			{1, -1, 1},
			{-1, -1, -1},
			{-1, 1, -1},
			{1, 1, -1},
			{1, -1, -1}
			});
		mesh.setColors({8, {1.0f, 1.0f, 1.0f, 1.0f}});
		mesh.setTriangles({
			1, 0, 3, 3, 2, 1,
			2, 3, 7, 7, 6, 2,
			3, 0, 4, 4, 7, 3,
			6, 5, 1, 1, 2, 6,
			4, 5, 6, 6, 7, 4,
			5, 4, 0, 0, 1, 5
			});
		return obj;
	}
}