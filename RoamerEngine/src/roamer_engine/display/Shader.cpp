#include "roamer_engine/display/Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace qy::cg {

	class Shader::SubShader {
	public:
		GLuint shader;

		SubShader(std::string_view code, int type, std::string_view name) {
			const char* shaderCode = code.data();
			// 2. compile shaders
			shader = glCreateShader(type);
			glShaderSource(shader, 1, &shaderCode, NULL);
			glCompileShader(shader);
			checkCompileErrors(name);
		}

		~SubShader() {
			// delete the shaders as they're linked into our program now and no longer necessary
			glDeleteShader(shader);
		}

		static SubShader load(const std::filesystem::path& path, int type, std::string_view name) {
			return SubShader(readShaderFile(path), type, name);
		}

		void attachTo(GLuint program) {
			glAttachShader(program, shader);
		}

		static std::string readShaderFile(const std::filesystem::path& path) {
			// 1. retrieve the vertex/fragment source code from filePath
			std::string code;
			std::ifstream shaderFile;
			// ensure ifstream objects can throw exceptions:
			shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try {
				// open files
				shaderFile.open(path);
				std::stringstream shaderStream;
				// read file's buffer contents into streams
				shaderStream << shaderFile.rdbuf();
				// close file handlers
				shaderFile.close();
				// convert stream into string
				code = shaderStream.str();
			} catch (std::ifstream::failure& e) {
				std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
			}
			return code;
		}

	private:

		// utility function for checking shader compilation/linking errors.
		void checkCompileErrors(std::string_view type) {
			int success;
			char infoLog[1024];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}

	};

	Shader Shader::fromSourceFile(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
		return fromSourceString(SubShader::readShaderFile(vertexPath), SubShader::readShaderFile(fragmentPath));
	}

	Shader Shader::fromSourceString(std::string_view vert, std::string_view frag) {
		Shader prog;
		prog.ID = glCreateProgram();
		SubShader(vert, GL_VERTEX_SHADER, "VERTEX").attachTo(prog.ID);
		SubShader(frag, GL_FRAGMENT_SHADER, "FRAGMENT").attachTo(prog.ID);
		glLinkProgram(prog.ID);
		prog.checkCompileErrors();
		++refCount[prog.ID];
		return prog;
	}

	void Shader::checkCompileErrors() {
		GLuint shader = ID;
		int success;
		char infoLog[1024];
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}

}
