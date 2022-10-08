#pragma once
#include "Renderer.hpp"
#include "Shaders.hpp"
#include <vector>

namespace qy::cg {
	class LineRenderer: public Renderer {

	public:
		using pointer = LineRenderer*;
		DECL_OBJECT(LineRenderer);

		pointer setShader(const ShaderProgram& shader);

		std::vector<glm::vec3> getPositions() const;

		pointer setPositions(const std::vector<glm::vec3>& positions);

		std::vector<glm::vec4> getColors() const;

		pointer setColors(const std::vector<glm::vec4>& colors);

		GLfloat getWidth() const;

		pointer setWidth(GLfloat width);

		GLfloat getLoop() const;

		pointer setLoop(bool loop);

		void render(const glm::mat4& mat) override;

	private:
		DECL_PIMPL;
	};
}