#include "roamer_engine/display/Mesh.hpp"

namespace qy::cg {

	struct Mesh::Impl {
		GLuint vao;
		GLuint vbo_Position;
		GLuint vbo_Color;
		GLuint ebo;
		GLuint vPositionLoc;
		GLuint vColorLoc;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec4> colors;
		std::vector<GLuint> triangles;

		Impl() {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo_Position);
			glGenBuffers(1, &vbo_Color);
			glGenBuffers(1, &ebo);
			vPositionLoc = 0;
			vColorLoc = 0;
		}
		~Impl() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo_Position);
			glDeleteBuffers(1, &vbo_Color);
			glDeleteBuffers(1, &ebo);
		}
	};

	DEFINE_OBJECT(Mesh);

	std::vector<glm::vec3> Mesh::getVertices() const {
		return pImpl->vertices;
	}

	void Mesh::setVertices(const std::vector<glm::vec3>& vertices) {
		pImpl->vertices = vertices;
		const float* data = reinterpret_cast<const float*>(vertices.data());
		glBindVertexArray(pImpl->vao);
		glBindBuffer(GL_ARRAY_BUFFER, pImpl->vbo_Position);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), data, GL_STATIC_DRAW);
		glVertexAttribPointer(pImpl->vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(pImpl->vPositionLoc);
	}

	std::vector<glm::vec4> Mesh::getColors() const {
		return pImpl->colors;
	}

	void Mesh::setColors(const std::vector<glm::vec4>& colors) {
		pImpl->colors = colors;
		const float* data = reinterpret_cast<const float*>(colors.data());
		glBindVertexArray(pImpl->vao);
		glBindBuffer(GL_ARRAY_BUFFER, pImpl->vbo_Color);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), data, GL_STATIC_DRAW);
		glVertexAttribPointer(pImpl->vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(pImpl->vColorLoc);
	}

	std::vector<GLuint> Mesh::getTriangles() const {
		return pImpl->triangles;
	}

	void Mesh::setTriangles(const std::vector<GLuint>& triangles) {
		pImpl->triangles = triangles;
		const GLuint* data = triangles.data();
		glBindVertexArray(pImpl->vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pImpl->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(GLuint), data, GL_STATIC_DRAW);
	}

	void Mesh::draw() const {
		glBindVertexArray(pImpl->vao);
		glDrawElements(GL_TRIANGLES, (GLsizei)pImpl->triangles.size(), GL_UNSIGNED_INT, 0);
	}

	void Mesh::setPositionLoc(GLuint loc) {
		pImpl->vPositionLoc = loc;
	}

	void Mesh::setColorLoc(GLuint loc) {
		pImpl->vColorLoc = loc;
	}
}