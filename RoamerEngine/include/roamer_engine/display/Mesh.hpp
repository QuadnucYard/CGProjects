#pragma once
#include <vector>
#include "../Object.hpp"

namespace qy::cg {

	enum class MeshTopology {
		Points = GL_POINTS, //Mesh is made from points.
		Lines = GL_LINES, // Mesh is made from lines.
		LineLoop = GL_LINE_LOOP,
		LineStrip = GL_LINE_STRIP, // Mesh is a line strip.
		Triangles = GL_TRIANGLES, // Mesh is made from triangles.
		TriangleStrip = GL_TRIANGLE_STRIP,
		TriangleFan = GL_TRIANGLE_FAN,
		LinesAdjacency = GL_LINES_ADJACENCY,
		LineStripAdjacency = GL_LINE_STRIP_ADJACENCY,
		TrangleAdjacency = GL_TRIANGLES_ADJACENCY,
		TrangleStripAdjacency = GL_TRIANGLE_STRIP_ADJACENCY,
		Patches = GL_PATCHES
	};

	class Mesh: Object {

		class MeshData;

	public:
		DECL_OBJECT(Mesh);

		std::vector<glm::vec3> getVertices() const;

		void setVertices(const std::vector<glm::vec3>& vertices);

		std::vector<glm::vec4> getColors() const;

		void setColors(const std::vector<glm::vec4>& colors);

		size_t getSubMeshCount() const;

		void setSubMeshCount(size_t count);

		size_t getIndexCount() const;

		size_t getIndexStart(size_t submesh) const;

		std::vector<GLuint> getIndices(size_t submesh) const;

		void setIndices(const std::vector<GLuint>& indices, MeshTopology topology, size_t submesh);

		std::vector<GLuint> getTriangles() const;

		void setTriangles(const std::vector<GLuint>& triangles);

		void uploadMeshData();

	private:
		void __render();

	private:
		friend class MeshRenderer;
		friend class Camera;
		DECL_PIMPL;
	};

}