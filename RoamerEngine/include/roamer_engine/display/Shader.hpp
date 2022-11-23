#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <unordered_map>

namespace qy::cg {

	namespace fs = std::filesystem;

	/*
	Shader program adapted form https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
	*/
	class Shader {

		class SubShader;

	private:
		GLuint ID;
		inline static std::unordered_map<GLuint, unsigned> refCount;

	public:

		Shader(): ID(0) {}
		Shader(const Shader& o): ID(o.ID) { ++refCount[o.ID]; }

		~Shader() {
			if (ID != 0 && --refCount[ID] == 0) glDeleteProgram(ID);
		}

		Shader& operator= (const Shader& o) {
			--refCount[ID];
			ID = o.ID;
			++refCount[ID];
			return *this;
		}

		/// @brief Create shader from source file.
		/// @param vertexPath Path of vertex shader.
		/// @param fragmentPath Path of fragment shader.
		/// @return Created shader.
		static Shader fromSourceFile(const fs::path& vertPath, const fs::path& fragPath);

		static Shader fromSourceFile(const fs::path& vertPath, const fs::path& fragPath, const fs::path& geomPath);

		/// @brief Create shader from string.
		/// @param vertexPath String of vertex shader.
		/// @param fragmentPath String of fragment shader.
		/// @return Created shader.
		static Shader fromSourceString(std::string_view vert, std::string_view frag);

		static Shader fromSourceString(std::string_view vert, std::string_view frag, std::string_view geom);

		// activate the shader
		inline void use() const {
			glUseProgram(ID);
		}

		// utility uniform functions
		inline void setBool(std::string_view name, bool value) const {
			glUniform1i(locAttrib(name), static_cast<int>(value));
		}

		inline void setInt(std::string_view name, int value) const {
			glUniform1i(locAttrib(name), value);
		}

		inline void setFloat(std::string_view name, float value) const {
			glUniform1f(locAttrib(name), value);
		}

		inline void setVec2(std::string_view name, const glm::vec2& value) const {
			glUniform2fv(locAttrib(name), 1, glm::value_ptr(value));
		}

		inline void setVec3(std::string_view name, const glm::vec3& value) const {
			glUniform3fv(locAttrib(name), 1, glm::value_ptr(value));
		}

		inline void setVec4(std::string_view name, const glm::vec4& value) const {
			glUniform4fv(locAttrib(name), 1, glm::value_ptr(value));
		}

		inline void setMat2(std::string_view name, const glm::mat2& value) const {
			glUniformMatrix2fv(locAttrib(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void setMat3(std::string_view name, const glm::mat3& value) const {
			glUniformMatrix3fv(locAttrib(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline void setMat4(std::string_view name, const glm::mat4& value) const {
			glUniformMatrix4fv(locAttrib(name), 1, GL_FALSE, glm::value_ptr(value));
		}

		inline GLint locAttrib(std::string_view name) const {
			return glGetUniformLocation(ID, name.data());
		}

	private:
		// utility function for checking shader compilation/linking errors.
		void checkCompileErrors();
	};
}
