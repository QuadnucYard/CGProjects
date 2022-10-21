#include "roamer_engine/display/MeshRenderer.hpp"

namespace qy::cg {

	struct MeshRenderer::Impl {
		ShaderProgram shader;
		Mesh mesh;
	};

	MeshRenderer::MeshRenderer() : MAKE_PIMPL(MeshRenderer) {
		setShader(Shaders::geom_unlit);
	}

	MeshRenderer::~MeshRenderer() = default;

	void MeshRenderer::setShader(const ShaderProgram& shader) {
		pImpl->shader = shader;
		pImpl->mesh.setPositionLoc(pImpl->shader.getAttribLocation("vPosition"));
		pImpl->mesh.setColorLoc(pImpl->shader.getAttribLocation("vColor"));
	}

	Mesh& MeshRenderer::getMesh() {
		return pImpl->mesh;
	}

	void MeshRenderer::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& proj) {
		pImpl->shader.use();
		pImpl->shader.setMat4("model", model);
		pImpl->shader.setMat4("view", view);
		pImpl->shader.setMat4("proj", proj);
		pImpl->mesh.draw();
	}
}