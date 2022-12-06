#pragma once
#include <glad/glad.h>
#include <string>
#include <unordered_map>

namespace qy::cg {

	class Buffers {
	private:
		inline static std::unordered_map<std::string, GLuint> buffers;

	public:
		static GLuint get(std::string& name) {
			auto it = buffers.find(name);
			if (it == buffers.end()) {
				GLuint id;
				glGenBuffers(1, &id);
				buffers.insert({name, id});
				return id;
			} else {
				return it->second;
			}
		}

	};
}