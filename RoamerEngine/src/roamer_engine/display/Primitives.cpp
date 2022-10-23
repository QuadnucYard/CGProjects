#include "roamer_engine/display/Primitives.hpp"
#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/Materials.hpp"
#include <numbers>

namespace qy::cg {

	Primitives::object_ptr Primitives::createCube() {
		auto obj = DisplayObject::create();
		auto mr = obj->addComponent<MeshRenderer>();
		mr->setMaterial(Materials::geom_unlit);
		auto mf = obj->addComponent<MeshFilter>();
		auto mesh = mf->mesh();
		mesh->setVertices({
			{-1, -1, 1},
			{-1, 1, 1},
			{1, 1, 1},
			{1, -1, 1},
			{-1, -1, -1},
			{-1, 1, -1},
			{1, 1, -1},
			{1, -1, -1} });
		mesh->setColors({ 8, {1.0f, 1.0f, 1.0f, 1.0f} });
		mesh->setIndices({
			1, 0, 3, 3, 2, 1,
			2, 3, 7, 7, 6, 2,
			3, 0, 4, 4, 7, 3,
			6, 5, 1, 1, 2, 6,
			4, 5, 6, 6, 7, 4,
			5, 4, 0, 0, 1, 5
			}, MeshTopology::Triangles, 0);
		return obj;
	}

	Primitives::object_ptr Primitives::createSphere() {
		auto obj = DisplayObject::create();
		auto mr = obj->addComponent<MeshRenderer>();
		auto&& mesh = mr->getMesh();

		const int phiDev = 50, thetaDev = 50;
		std::vector<glm::vec3> vertices((phiDev + 1) * thetaDev);
		std::vector<GLuint> trianles(phiDev * thetaDev * 3 * 2);
		std::vector<glm::vec4> color = { (phiDev + 1) * thetaDev , {1.0f, 1.0f, 1.0f, 1.0f} };

		using namespace std::numbers;
		for (int i = 0; i <= phiDev; i++) {
			for (int j = 0; j < thetaDev; j++) {
				float xPos = sin(pi * i / phiDev) * cos(pi * 2 * j / thetaDev);
				float yPos = sin(pi * i / phiDev) * sin(pi * 2 * j / thetaDev);
				float zPos = cos(pi * i / phiDev);
				vertices[i * thetaDev + j] = glm::vec3(xPos, yPos, zPos);
			}
		}
		for (int i = 0; i < phiDev; i++) {
			for (int j = 0; j < thetaDev; j++) {
				trianles[(i * thetaDev + j) * 6 + 0] = i * thetaDev + j;
				trianles[(i * thetaDev + j) * 6 + 1] = (i + 1) * thetaDev + (j + 1) % thetaDev;
				trianles[(i * thetaDev + j) * 6 + 2] = (i + 1) * thetaDev + j;
				trianles[(i * thetaDev + j) * 6 + 3] = i * thetaDev + j;
				trianles[(i * thetaDev + j) * 6 + 4] = i * thetaDev + (j + 1) % thetaDev;
				trianles[(i * thetaDev + j) * 6 + 5] = (i + 1) * thetaDev + (j + 1) % thetaDev;
			}
		}

		using namespace qy::cg;
		mesh.setVertices(vertices);
		mesh.setTriangles(trianles);
		mesh.setColors(color);
		return obj;
	}
}