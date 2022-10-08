#pragma once

#include <vector>
#include "../Object.hpp"

namespace qy::cg {

	class Mesh : Object {

	public:
		DECL_OBJECT(Mesh);

		std::vector<glm::vec3> getVertices() const;

		void setVertices(const std::vector<glm::vec3>& vertices);

		std::vector<glm::vec4> getColors() const;

		void setColors(const std::vector<glm::vec4>& colors);

		std::vector<GLuint> getTriangles() const;

		void setTriangles(const std::vector<GLuint>& triangles);

		void draw() const;

	private:
		void setPositionLoc(GLuint loc);
		void setColorLoc(GLuint loc);

	private:
		friend class MeshRenderer;
		DECL_PIMPL;
	};

}