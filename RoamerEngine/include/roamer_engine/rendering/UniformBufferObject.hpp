#pragma once
#include <glad/glad.h>

namespace qy::cg::rendering {

	template <class T>
	class UniformBufferObject {

	public:

		UniformBufferObject(GLuint index) : index(index), data {} {
			glGenBuffers(1, &ubo);
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(T), nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, index, ubo);
			//glBindBufferRange(GL_UNIFORM_BUFFER, index, ubo, 0, sizeof(T));
		}

		~UniformBufferObject() {
			glDeleteBuffers(1, &ubo);
		}

		void upload() {
			glBindBuffer(GL_UNIFORM_BUFFER, ubo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T), &data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		const T* operator-> () const { return &data; }
		T* operator-> () { return &data; }

	private:
		GLuint ubo;
		GLuint index;
		T data;
	};

}