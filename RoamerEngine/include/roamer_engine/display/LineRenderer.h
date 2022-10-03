#pragma once

#include "Renderer.h"
#include "Shaders.h"
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
			setShader(Shaders::geom_unlit);
		}

		pointer setShader(const ShaderProgram& shader) {
			_shader = shader;
			_impl.vPositionLoc = _shader.getAttribLocation("vPosition");
			_impl.vColorLoc = _shader.getAttribLocation("vColor");
			return this;
		}

		std::vector<glm::vec3> getPositions() const {
			return _impl.vertices;
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

		std::vector<glm::vec4> getColors() const {
			return _impl.colors;
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

		GLfloat getWidth() const {
			return _width;
		}

		pointer setWidth(GLfloat width) {
			_width = width;
			return this;
		}

		GLfloat getLoop() const {
			return _loop;
		}

		pointer setLoop(bool loop) {
			_loop = loop;
			return this;
		}

		void render(const glm::mat4& mat) override {
			_shader.use();
			_shader.setMat4("transform", mat);
			glBindVertexArray(_impl.vao);
			glLineWidth(_width);
			if (_loop) glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)_impl.vertices.size());
			else glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)_impl.vertices.size());
		}
	};
}