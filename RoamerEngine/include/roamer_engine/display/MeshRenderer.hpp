#pragma once
#include "Renderer.hpp"
#include "Shaders.hpp"

namespace qy::cg {
	class MeshRenderer: public Renderer {

	public:
		DECL_OBJECT(MeshRenderer);

		void setShader(const ShaderProgram& shader);

		void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) override;

	private:
		DECL_PIMPL;
	};
}