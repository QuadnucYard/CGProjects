#include "roamer_engine/display/MeshRenderer.hpp"
#include "roamer_engine/display/MeshFilter.hpp"

namespace qy::cg {

	struct MeshRenderer::Impl {
		ShaderProgram shader;
	};

	MeshRenderer::MeshRenderer() : MAKE_PIMPL(MeshRenderer) {
		setShader(Shaders::geom_unlit);
	}

	MeshRenderer::~MeshRenderer() = default;

	void MeshRenderer::setShader(const ShaderProgram& shader) {
		pImpl->shader = shader;
	}

	void MeshRenderer::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) {
		pImpl->shader.use();
		pImpl->shader.setMat4("model", model);
		pImpl->shader.setMat4("view", view);
		pImpl->shader.setMat4("proj", proj);
		//getComponent<MeshFilter>()->mesh()->__render();
	}
}