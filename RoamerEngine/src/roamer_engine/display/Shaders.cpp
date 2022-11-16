#include "roamer_engine/display/Shaders.hpp"

namespace qy::cg {
	void Shaders::__INIT__() {
		auto shaderPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";

#define INIT_SHADER(shader, path) \
				shader = Shader::fromSourceFile(shaderPath / path##".vert", shaderPath / path##".frag");

		INIT_SHADER(Unlit, "unlit");
		INIT_SHADER(Skybox, "skybox");
		INIT_SHADER(Lit, "lit");

#undef INIT_SHADER
	}
}