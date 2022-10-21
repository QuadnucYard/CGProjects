#pragma once
#include "Renderer.hpp"
#include "Shaders.hpp"
#include "Mesh.hpp"

namespace qy::cg {
	class MeshRenderer: public Renderer {

	public:
		DECL_OBJECT(MeshRenderer);

		void setShader(const ShaderProgram& shader);

		Mesh& getMesh();

		void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) override;

	private:
		DECL_PIMPL;
	};
}