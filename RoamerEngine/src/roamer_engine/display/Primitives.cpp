#include "roamer_engine/display/Primitives.hpp"
#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/Mesh.hpp"
#include "roamer_engine/display/Materials.hpp"
#include <numbers>

namespace qy::cg {

	Primitives::object_ptr Primitives::createCube() {
		auto obj = DisplayObject::create();
		obj->addComponent<MeshRenderer>()->setMaterial(Materials::GeomUnlit);
		auto mesh = obj->addComponent<MeshFilter>()->mesh();
		mesh->setVertices({
			{-1, -1, 1},
			{-1, 1, 1},
			{1, 1, 1},
			{1, -1, 1},
			{-1, -1, -1},
			{-1, 1, -1},
			{1, 1, -1},
			{1, -1, -1}});
		mesh->setColors({8, {1.0f, 1.0f, 1.0f, 1.0f}});
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
		obj->addComponent<MeshRenderer>()->setMaterial(Materials::GeomUnlit);
		auto mesh = obj->addComponent<MeshFilter>()->mesh();

		const size_t numDiv = 50, vertNum = (numDiv + 1) * (numDiv + 1);
		std::vector<glm::vec3> vertices(vertNum);
		std::vector<glm::vec3> normals(vertNum);
		std::vector<glm::vec4> tangents(vertNum);
		std::vector<glm::vec2> uvs(vertNum);
		std::vector<GLuint> triangles(numDiv * numDiv * 3 * 2);
		std::vector<glm::vec4> colors(vertNum, {1.0f, 1.0f, 1.0f, 1.0f});

		const float pi = std::numbers::pi_v<float>;
		for (size_t i = 0; i <= numDiv; i++) {
			for (size_t j = 0; j <= numDiv; j++) {
				float phi = pi * i / numDiv;
				float theta = pi * 2 * j / numDiv;
				float yPos = cos(phi);
				float xPos = - sin(phi) * cos(theta);
				float zPos = sin(phi) * sin(theta);
				vertices[i * (numDiv + 1) + j] = {xPos, yPos, zPos};
				normals[i * (numDiv + 1) + j] = {xPos, yPos, zPos};
				uvs[i * (numDiv + 1) + j] = {(float)j / numDiv, (float)i / numDiv};

				if (xPos == 0 && zPos == 0 && (yPos == 1 || yPos == -1)) {
					tangents[i * (numDiv + 1) + j] = {0.0f, 0.0f, -1.0f, 1.0f};
				} else {
					tangents[i * (numDiv + 1) + j] = {glm::cross({0.0f, 1.0f, 0.0f}, glm::vec3 {xPos, yPos, zPos}), 1.0f};
				}
			}
		}
		for (size_t i = 0; i < numDiv; i++) {
			for (size_t j = 0; j < numDiv; j++) {
				triangles[(i * numDiv + j) * 6 + 0] = i * (numDiv + 1) + j;
				triangles[(i * numDiv + j) * 6 + 1] = (i + 1) * (numDiv + 1) + j;
				triangles[(i * numDiv + j) * 6 + 2] = i * (numDiv + 1) + j + 1;
				triangles[(i * numDiv + j) * 6 + 3] = i * (numDiv + 1) + j + 1;
				triangles[(i * numDiv + j) * 6 + 4] = (i + 1) * (numDiv + 1) + j;
				triangles[(i * numDiv + j) * 6 + 5] = (i + 1) * (numDiv + 1) + j + 1;
			}
		}

		mesh->setVertices(vertices);
		mesh->setTangents(tangents);
		mesh->setUVs(uvs);
		mesh->setTriangles(triangles);
		mesh->setColors(colors);
		return obj;
	}
}