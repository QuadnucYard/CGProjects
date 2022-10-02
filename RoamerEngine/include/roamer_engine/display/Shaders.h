#pragma once
#include "ShaderProgram.h"

namespace qy::cg {

	class Shaders {
	public:
		inline static ShaderProgram geom_unlit;

		static void __INIT__() {
			auto shaderPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";
#define INIT_SHADER(name, path) \
				name = ShaderProgram(shaderPath / path##".vert", shaderPath / path##".frag");
			INIT_SHADER(geom_unlit, "geom-unlit");
#undef INIT_SHADER
		}
	};
}