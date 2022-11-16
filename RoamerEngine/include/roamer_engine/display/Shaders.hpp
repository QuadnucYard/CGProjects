#pragma once
#include "Shader.hpp"

namespace qy::cg {

	class Shaders {
	public:
		inline static Shader geom_unlit; // Obsoleted
		inline static Shader skybox; // Obsoleted
		inline static Shader GeomUnlit;
		inline static Shader Skybox;
		inline static Shader Lit;

		static void __INIT__() {
			auto shaderPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";

#define INIT_SHADER(shader, path) \
				shader = Shader::fromSourceFile(shaderPath / path##".vert", shaderPath / path##".frag");

			INIT_SHADER(GeomUnlit, "geom-unlit");
			INIT_SHADER(Skybox, "skybox");
			INIT_SHADER(Lit, "lit");

			geom_unlit = GeomUnlit;
			skybox = Skybox;

#undef INIT_SHADER
		}
	};
}