#include "roamer_engine/display/Primitives.hpp"
#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/Mesh.hpp"
#include "roamer_engine/display/Materials.hpp"
#include <numbers>

namespace qy::cg {

	Primitives::object_ptr Primitives::createCube() {
		auto obj = DisplayObject::create();
		obj->addComponent<MeshRenderer>()->setMaterial(Materials::Unlit);
		auto mesh = obj->addComponent<MeshFilter>()->mesh();
		static size_t vertNum = 24;
		mesh->setVertices({
			{-1, -1, 1}, {-1, 1, 1}, {1, 1, 1}, {1, -1, 1},
			{1, -1, 1}, {1, 1, 1}, {1, 1, -1},{1, -1, -1},
			{1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, -1},
			{-1,-1,-1}, {-1, 1, -1}, {-1, 1, 1}, {-1,-1,1},
			{-1,1,1}, {-1,1,-1}, {1, 1,-1}, {1, 1, 1},
			{-1,-1,-1}, {-1, -1, 1}, {1,-1,1}, {1,-1,-1},
			});
		mesh->setColors({vertNum, {1.0f, 1.0f, 1.0f, 1.0f}});
		mesh->setIndices({
			1, 0, 3, 3, 2, 1,
			5, 4, 7, 7, 6, 5,
			9, 8, 11, 11, 10, 9,
			13, 12, 15, 15, 14, 13,
			17, 16, 19, 19, 18, 17,
			21, 20, 23, 23, 22, 21
			}, MeshTopology::Triangles, 0);
		mesh->setNormals({
			{0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
			{1, 0, 0}, {1, 0, 0}, {1, 0, 0},{1, 0, 0},
			{0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
			{-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
			{0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
			{0,-1, 0}, {0, -1, 0}, {0,-1,0}, {0,-1, 0},
			});
		mesh->setUVs({
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0},
			{0,0},{0,1},{1,1},{1,0}
			});
		return obj;
	}

	Primitives::object_ptr Primitives::createSphere() {
		auto obj = DisplayObject::create();
		obj->addComponent<MeshRenderer>()->setMaterial(Materials::Unlit);
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
				triangles[(i * numDiv + j) * 6 + 0] = GLuint(i * (numDiv + 1) + j);
				triangles[(i * numDiv + j) * 6 + 1] = GLuint((i + 1) * (numDiv + 1) + j);
				triangles[(i * numDiv + j) * 6 + 2] = GLuint(i * (numDiv + 1) + j + 1);
				triangles[(i * numDiv + j) * 6 + 3] = GLuint(i * (numDiv + 1) + j + 1);
				triangles[(i * numDiv + j) * 6 + 4] = GLuint((i + 1) * (numDiv + 1) + j);
				triangles[(i * numDiv + j) * 6 + 5] = GLuint((i + 1) * (numDiv + 1) + j + 1);
			}
		}

		mesh->setVertices(vertices);
		mesh->setTangents(tangents);
		mesh->setUVs(uvs);
		mesh->setTriangles(triangles);
		mesh->setColors(colors);
		mesh->setNormals(normals);
		return obj;
	}
	Primitives::object_ptr Primitives::createCylinder()
	{
		const size_t numDiv = 50, vertNum = numDiv * 4 + 2;
		auto obj = DisplayObject::create();
		obj->addComponent<MeshRenderer>()->setMaterial(Materials::Unlit);
		auto mesh = obj->addComponent<MeshFilter>()->mesh();

		std::vector<glm::vec3> vertices(vertNum);
		std::vector<GLuint> triangles(numDiv * 4 * 3);
		std::vector<glm::vec4> vcolors(vertNum, { 1.0f, 1.0f, 1.0f, 1.0f });
		
		std::vector<glm::vec3> normals(vertNum);
		//std::vector<glm::vec4> tangents(vertNum);
		//std::vector<glm::vec2> uvs(vertNum);
		
		vertices[numDiv] = { 0, -1, 0 };
		vertices[2 * numDiv + 1] = { 0, 1, 0 };

		const float pi = std::numbers::pi_v<float>;
		for (int i = 0; i < numDiv; i++) {
			vertices[i] = glm::vec3(cos(pi * 2 * i / numDiv), -1, sin(pi * 2 * i / numDiv));
			triangles[i * 12 + 0] = numDiv;
			triangles[i * 12 + 1] = i % numDiv;
			triangles[i * 12 + 2] = (i + 1) % numDiv;
			normals[i] = { 0, -1, 0 };

			vertices[numDiv + i + 1] = glm::vec3(cos(pi * 2 * i / numDiv), 1, sin(pi * 2 * i / numDiv));
			triangles[i * 12 + 3] = numDiv * 2 + 1;
			triangles[i * 12 + 4] = (i + 1) % numDiv + numDiv + 1;
			triangles[i * 12 + 5] = i % numDiv + numDiv + 1;
			normals[numDiv + i + 1] = { 0, 1, 0 };

			vertices[2 * numDiv + 2 + i] = glm::vec3(cos(pi * 2 * i / numDiv), -1, sin(pi * 2 * i / numDiv));
			normals[2 * numDiv + 2 + i] = glm::vec3(cos(pi * 2 * i / numDiv), 0, sin(pi * 2 * i / numDiv));
			triangles[i * 12 + 6] = i % numDiv + 2*numDiv + 2;
			triangles[i * 12 + 7] = i % numDiv + 3*numDiv + 2;
			triangles[i * 12 + 8] = (i + 1) % numDiv + 3*numDiv + 2;

			vertices[3 * numDiv + 2 + i] = glm::vec3(cos(pi * 2 * i / numDiv), 1, sin(pi * 2 * i / numDiv));
			normals[3 * numDiv + 2 + i] = glm::vec3(cos(pi * 2 * i / numDiv), 0, sin(pi * 2 * i / numDiv));
			triangles[i * 12 + 9] = i % numDiv + 2 * numDiv + 2;
			triangles[i * 12 + 10] = (i + 1) % numDiv + 3 * numDiv + 2;
			triangles[i * 12 + 11] = (i + 1) % numDiv + 2 * numDiv + 2;
		}

		mesh->setVertices(vertices);
		mesh->setTriangles(triangles);
		mesh->setColors(vcolors);
		mesh->setNormals(normals);
		return obj;
		
	}
}