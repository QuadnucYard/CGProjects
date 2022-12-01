#pragma once
#include "Shader.hpp"

namespace qy::cg {

	class Shaders {
	public:
		static fs::path ResPath;

		inline static Shader Unlit;
		inline static Shader Skybox;
		inline static Shader Lit;

		static void __INIT__();
	};
}