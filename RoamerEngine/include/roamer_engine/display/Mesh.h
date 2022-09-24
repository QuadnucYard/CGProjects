#pragma once

#include <vector>
#include "../Object.h"

namespace qy::cg {

	// 网格，目前仅支持一种
	class Mesh : Object {

		struct _Impl {
			GLuint vao;
			GLuint vbo_Position;
			GLuint vbo_Color;
			GLuint ebo;
			GLuint vPositionLoc;
			GLuint vColorLoc;
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec4> colors;
			std::vector<GLuint> triangles;

			_Impl() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo_Position);
				glGenBuffers(1, &vbo_Color);
				glGenBuffers(1, &ebo);
				vPositionLoc = 0;
				vColorLoc = 0;
			}
			~_Impl() {
				glDeleteVertexArrays(1, &vao);
				glDeleteBuffers(1, &vbo_Position);
				glDeleteBuffers(1, &vbo_Color);
				glDeleteBuffers(1, &ebo);
			}
		};

		std::shared_ptr<_Impl> _impl = std::make_shared<_Impl>();

	public:

		void setVertices(const std::vector<glm::vec3>& vertices) {
			_impl->vertices = vertices;
			const float* data = reinterpret_cast<const float*>(vertices.data());
			glBindVertexArray(_impl->vao);
			glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo_Position);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), data, GL_STATIC_DRAW);
			glVertexAttribPointer(_impl->vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(_impl->vPositionLoc);
		}

		void setColors(const std::vector<glm::vec4>& colors) {
			_impl->colors = colors;
			const float* data = reinterpret_cast<const float*>(colors.data());
			glBindVertexArray(_impl->vao);
			glBindBuffer(GL_ARRAY_BUFFER, _impl->vbo_Color);
			glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), data, GL_STATIC_DRAW);
			glVertexAttribPointer(_impl->vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(_impl->vColorLoc);
		}

		void setTriangles(const std::vector<GLuint>& triangles) {
			_impl->triangles = triangles;
			const GLuint* data = triangles.data();
			glBindVertexArray(_impl->vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_impl->ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(GLuint), data, GL_STATIC_DRAW);
		}

		void draw() const {
			glBindVertexArray(_impl->vao);
			glDrawElements(GL_TRIANGLES, _impl->triangles.size(), GL_UNSIGNED_INT, 0);
		}

	private:

		friend class MeshRenderer;

	};

}