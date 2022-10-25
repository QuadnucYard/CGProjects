#pragma once
#include "Shader.hpp"

namespace qy::cg {

	class Shaders {
	public:
		inline static Shader geom_unlit;
		inline static Shader skybox;

		static void __INIT__() {
			auto shaderPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";

#define INIT_SHADER(shader, path) \
				shader = Shader::fromSourceFile(shaderPath / path##".vert", shaderPath / path##".frag");

			INIT_SHADER(geom_unlit, "geom-unlit");
			INIT_SHADER(skybox, "skybox");

#undef INIT_SHADER
		}
	};
}