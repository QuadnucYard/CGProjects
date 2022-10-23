#pragma once
#include "Shader.hpp"

namespace qy::cg {

	class Shaders {
	public:
		inline static Shader geom_unlit;

		static void __INIT__() {
			auto shaderPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";
#define INIT_SHADER(name, path) \
				name = Shader(shaderPath / path##".vert", shaderPath / path##".frag");
			INIT_SHADER(geom_unlit, "geom-unlit");
#undef INIT_SHADER
		}
	};
}