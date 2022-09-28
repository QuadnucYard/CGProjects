#pragma once

#include "Component.h"
#include "Renderer.h"
#include "ShaderProgram.h"
#include <vector>

namespace qy::cg {
	class LineRenderer: public Renderer {

	private:
		struct _Impl {
			GLuint vao;
			GLuint vbo_Position;
			GLuint vbo_Color;
			GLuint vPositionLoc {0};
			GLuint vColorLoc {0};
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec4> colors;

			_Impl() {
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo_Position);
				glGenBuffers(1, &vbo_Color);
			}
			~_Impl() {
				glDeleteVertexArrays(1, &vao);
				glDeleteBuffers(1, &vbo_Position);
				glDeleteBuffers(1, &vbo_Color);
			}
		} _impl;

		ShaderProgram _shader;
		bool _loop;
		GLfloat _width;

	public:
		using pointer = LineRenderer*;

		LineRenderer() {
			_loop = false;
			_width = 1.0f;
		}

		pointer setShader(const ShaderProgram& shader) {
			_shader = shader;
			_impl.vPositionLoc = _shader.getAttribLocation("vPosition");
			_impl.vColorLoc = _shader.getAttribLocation("vColor");
			return this;
		}

		pointer setPositions(const std::vector<glm::vec3>& positions) {
			_impl.vertices = positions;
			const float* data = reinterpret_cast<const float*>(positions.data());
			glBindVertexArray(_impl.vao);
			glBindBuffer(GL_ARRAY_BUFFER, _impl.vbo_Position);
			glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), data, GL_STATIC_DRAW);
			glVertexAttribPointer(_impl.vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(_impl.vPositionLoc);
			return this;
		}

		pointer setColors(const std::vector<glm::vec4>& colors) {
			_impl.colors = colors;
			const float* data = reinterpret_cast<const float*>(colors.data());
			glBindVertexArray(_impl.vao);
			glBindBuffer(GL_ARRAY_BUFFER, _impl.vbo_Color);
			glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), data, GL_STATIC_DRAW);
			glVertexAttribPointer(_impl.vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(_impl.vColorLoc);
			return this;
		}

		pointer setWidth(GLfloat width) {
			_width = width;
			return this;
		}

		pointer setLoop(bool loop) {
			_loop = loop;
			return this;
		}

		void render() override {
			glBindVertexArray(_impl.vao);
			glLineWidth(_width);
			if (_loop) glDrawArrays(GL_LINE_LOOP, 0, _impl.vertices.size());
			else glDrawArrays(GL_LINE_STRIP, 0, _impl.vertices.size());
		}
	};
}