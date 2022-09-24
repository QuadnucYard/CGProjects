#pragma once
#include "Component.h"
#include "IRenderer.h"
#include "ShaderProgram.h"
#include "Mesh.h"

namespace qy::cg {
	class MeshRenderer: Component, IRenderer {

	private:
		ShaderProgram _shader;
		Mesh _mesh;

	public:
		void setShader(const ShaderProgram& shader) {
			_shader = shader;
			_mesh._impl->vPositionLoc = _shader.getAttribLocation("vPosition");
			_mesh._impl->vColorLoc = _shader.getAttribLocation("vColor");
		}

		Mesh& getMesh() {
			return _mesh;
		}

		void render() override {
			_shader.use();
			_mesh.draw();
		}
	};
}