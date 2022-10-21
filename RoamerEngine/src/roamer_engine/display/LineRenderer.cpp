#include "roamer_engine/display/LineRenderer.hpp"

namespace qy::cg {

	struct LineRenderer::Impl {
		GLuint vao;
		GLuint vbo_Position;
		GLuint vbo_Color;
		GLuint vPositionLoc {0};
		GLuint vColorLoc {0};
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec4> colors;

		ShaderProgram shader {};
		bool loop;
		GLfloat width;

		Impl() {
			loop = false;
			width = 1;
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo_Position);
			glGenBuffers(1, &vbo_Color);
		}
		~Impl() {
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo_Position);
			glDeleteBuffers(1, &vbo_Color);
		}
	};

	LineRenderer::LineRenderer(): MAKE_PIMPL(LineRenderer) {
		setShader(Shaders::geom_unlit);
	}

	LineRenderer* LineRenderer::setShader(const ShaderProgram& shader) {
		pImpl->shader = shader;
		pImpl->vPositionLoc = pImpl->shader.getAttribLocation("vPosition");
		pImpl->vColorLoc = pImpl->shader.getAttribLocation("vColor");
		return this;
	}

	std::vector<glm::vec3> LineRenderer::getPositions() const {
		return pImpl->vertices;
	}

	LineRenderer* LineRenderer::setPositions(const std::vector<glm::vec3>& positions) {
		pImpl->vertices = positions;
		const float* data = reinterpret_cast<const float*>(positions.data());
		glBindVertexArray(pImpl->vao);
		glBindBuffer(GL_ARRAY_BUFFER, pImpl->vbo_Position);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), data, GL_STATIC_DRAW);
		glVertexAttribPointer(pImpl->vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(pImpl->vPositionLoc);
		return this;
	}

	std::vector<glm::vec4> LineRenderer::getColors() const {
		return pImpl->colors;
	}

	LineRenderer* LineRenderer::setColors(const std::vector<glm::vec4>& colors) {
		pImpl->colors = colors;
		const float* data = reinterpret_cast<const float*>(colors.data());
		glBindVertexArray(pImpl->vao);
		glBindBuffer(GL_ARRAY_BUFFER, pImpl->vbo_Color);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), data, GL_STATIC_DRAW);
		glVertexAttribPointer(pImpl->vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(pImpl->vColorLoc);
		return this;
	}

	GLfloat LineRenderer::getWidth() const {
		return pImpl->width;
	}

	LineRenderer* LineRenderer::setWidth(GLfloat width) {
		pImpl->width = width;
		return this;
	}

	GLfloat LineRenderer::getLoop() const {
		return pImpl->loop;
	}

	LineRenderer* LineRenderer::setLoop(bool loop) {
		pImpl->loop = loop;
		return this;
	}

	void LineRenderer::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) {
		pImpl->shader.use();
		pImpl->shader.setMat4("model", model);
		pImpl->shader.setMat4("view", view);
		pImpl->shader.setMat4("proj", proj);
		glBindVertexArray(pImpl->vao);
		glLineWidth(pImpl->width);
		glDrawArrays(pImpl->loop ? GL_LINE_LOOP : GL_LINE_STRIP, 0, (GLsizei)pImpl->vertices.size());
	}
}