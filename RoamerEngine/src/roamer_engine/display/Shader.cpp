#include "roamer_engine/display/Shader.hpp"
#include "roamer_engine/utils/ShaderCompiler.hpp"
#include <iostream>

namespace qy::cg {

	class Shader::SubShader {
	public:
		GLuint shader;

		SubShader(const fs::path& path, int type, std::string_view name) {
			static ShaderCompiler compiler;
			const std::string shaderCode = compiler.compile(path);
			const char* shaderCode2 = shaderCode.data();
			// 2. compile shaders
			shader = glCreateShader(type);
			glShaderSource(shader, 1, &shaderCode2, nullptr);
			glCompileShader(shader);
			checkCompileErrors(name);
		}

		~SubShader() {
			// delete the shaders as they're linked into our program now and no longer necessary
			glDeleteShader(shader);
		}

		void attachTo(GLuint program) const {
			glAttachShader(program, shader);
		}

	private:

		// utility function for checking shader compilation/linking errors.
		void checkCompileErrors(std::string_view type) const {
			int success;
			char infoLog[1024];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}

	};

	Shader::Shader(const fs::path& vert, const fs::path& frag) {
		ID = glCreateProgram();
		SubShader(vert, GL_VERTEX_SHADER, "VERTEX").attachTo(ID);
		SubShader(frag, GL_FRAGMENT_SHADER, "FRAGMENT").attachTo(ID);
		glLinkProgram(ID);
		checkCompileErrors();
	}

	Shader::Shader(const fs::path& vert, const fs::path& frag, const fs::path& geom) {
		ID = glCreateProgram();
		SubShader(vert, GL_VERTEX_SHADER, "VERTEX").attachTo(ID);
		SubShader(frag, GL_FRAGMENT_SHADER, "FRAGMENT").attachTo(ID);
		SubShader(geom, GL_GEOMETRY_SHADER, "GEOMERTY").attachTo(ID);
		glLinkProgram(ID);
		checkCompileErrors();
	}

	void Shader::checkCompileErrors() {
		int success;
		char infoLog[1024];
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

}
