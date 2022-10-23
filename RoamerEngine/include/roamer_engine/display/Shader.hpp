#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string_view>

namespace qy::cg {

	/*
	Shader program adapted form https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
	*/
	class Shader {

		class SubShader;

	private:
		GLuint ID;

	public:

		Shader() { ID = 0; }

		// constructor generates the shader on the fly
		Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

		~Shader() {
			//glDeleteProgram(ID);
		}

		static Shader create(std::string_view vert, std::string_view frag);

		// activate the shader
		void use() const {
			glUseProgram(ID);
		}

		// utility uniform functions
		void setBool(std::string_view name, bool value) const {
			glUniform1i(locAttrib(name), static_cast<int>(value));
		}

		void setInt(std::string_view name, int value) const {
			glUniform1i(locAttrib(name), value);
		}
	
		void setFloat(std::string_view name, float value) const {
			glUniform1f(locAttrib(name), value);
		}

		void setVec2(std::string_view name, const glm::vec2& value) const {
			glUniform2fv(locAttrib(name), 1, glm::value_ptr(value));
		}

		void setVec3(std::string_view name, const glm::vec3& value) const {
			glUniform3fv(locAttrib(name), 1, glm::value_ptr(value));
		}

		void setVec4(std::string_view name, const glm::vec4& value) const {
			glUniform4fv(locAttrib(name), 1, glm::value_ptr(value));
		}

		void setMat2(std::string_view name, const glm::mat2& value) const {
			glUniformMatrix2fv(locAttrib(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void setMat3(std::string_view name, const glm::mat3& value) const {
			glUniformMatrix3fv(locAttrib(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		void setMat4(std::string_view name, const glm::mat4& value) const {
			glUniformMatrix4fv(locAttrib(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		GLint locAttrib(std::string_view name) const {
			return glGetAttribLocation(ID, name.data());
		}

	private:
		// utility function for checking shader compilation/linking errors.
		void checkCompileErrors();
	};
}
