#include "roamer_engine/display/Shaders.hpp"

namespace qy::cg {
	fs::path Shaders::ResPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";

	void Shaders::__INIT__() {

#define INIT_SHADER(shader, path) \
				shader = Shader::fromSourceFile(ResPath / path##".vert", ResPath / path##".frag");

		INIT_SHADER(Unlit, "unlit");
		INIT_SHADER(Skybox, "skybox");
		INIT_SHADER(Lit, "lit");

#undef INIT_SHADER
	}
}