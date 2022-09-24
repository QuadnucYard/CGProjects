#pragma once

#include <gl/glew.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string_view>

namespace qy::cg {

	class Shader
	{
	public:
		GLuint shader;

		Shader(const std::filesystem::path& path, int type, std::string_view name)
		{
			std::string code = readShaderFile(path);
			const char* shaderCode = code.c_str();
			// 2. compile shaders
			shader = glCreateShader(type);
			glShaderSource(shader, 1, &shaderCode, NULL);
			glCompileShader(shader);
			checkCompileErrors(shader, name);
		}

		~Shader()
		{
			// delete the shaders as they're linked into our program now and no longer necessary
			glDeleteShader(shader);
		}

		void attachTo(GLuint program)
		{
			glAttachShader(program, shader);
		}

	private:

		std::string readShaderFile(const std::filesystem::path& path)
		{
			// 1. retrieve the vertex/fragment source code from filePath
			std::string code;
			std::ifstream shaderFile;
			// ensure ifstream objects can throw exceptions:
			shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files
				shaderFile.open(path);
				std::stringstream shaderStream;
				// read file's buffer contents into streams
				shaderStream << shaderFile.rdbuf();
				// close file handlers
				shaderFile.close();
				// convert stream into string
				code = shaderStream.str();
			} catch (std::ifstream::failure& e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
			}
			return code;
		}

		// utility function for checking shader compilation/linking errors.
		// ------------------------------------------------------------------------
		void checkCompileErrors(GLuint shader, std::string_view type)
		{
			int success;
			char infoLog[1024];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}

	};

	/*
	Shader program adapted form https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
	*/
	class ShaderProgram
	{
	private:
		GLuint ID;

	public:

		ShaderProgram() = default;

		// constructor generates the shader on the fly
		// ------------------------------------------------------------------------
		ShaderProgram(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
		{
			// shader Program
			ID = glCreateProgram();
			Shader(vertexPath, GL_VERTEX_SHADER, "VERTEX").attachTo(ID);
			Shader(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT").attachTo(ID);
			glLinkProgram(ID);
			checkCompileErrors(ID);
		}

		~ShaderProgram() {
			//glDeleteProgram(ID);
		}

		// activate the shader
		// ------------------------------------------------------------------------
		void use() const
		{
			glUseProgram(ID);
		}
		// utility uniform functions
		// ------------------------------------------------------------------------
		void setBool(std::string_view name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.data()), (int)value);
		}
		// ------------------------------------------------------------------------
		void setInt(std::string_view name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.data()), value);
		}
		// ------------------------------------------------------------------------
		void setFloat(std::string_view name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.data()), value);
		}

		GLuint getAttribLocation(std::string_view name) const
		{
			return glGetAttribLocation(ID, name.data());
		}

	private:
		// utility function for checking shader compilation/linking errors.
		// ------------------------------------------------------------------------
		void checkCompileErrors(GLuint shader)
		{
			int success;
			char infoLog[1024];
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	};
}
