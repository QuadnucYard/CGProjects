#pragma once
#include "../Object.hpp"
#include "ShaderProgram.hpp"

namespace qy::cg {

	class Material: public Object {

		ShaderProgram m_shader;
	public:
		const ShaderProgram& getShader() { return m_shader; }
		void setShader(const ShaderProgram& shader) { m_shader = shader; }
	};
}