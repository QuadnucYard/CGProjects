#include "roamer_engine/display/Shaders.hpp"

namespace qy::cg {
	fs::path Shaders::ResPath = std::filesystem::current_path().parent_path() / "RoamerEngine" / "shaders";

	void Shaders::__INIT__() {

#define INIT_SHADER_VF(shader, path) \
		shader = Shader(ResPath / path##".vert", ResPath / path##".frag");
#define INIT_SHADER_VFG(shader, path) \
		shader = Shader(ResPath / path##".vert", ResPath / path##".frag", ResPath / path##".geom");

		INIT_SHADER_VF(Unlit, "unlit");
		INIT_SHADER_VF(Skybox, "skybox");
		INIT_SHADER_VF(Lit, "lit");
		INIT_SHADER_VF(DirectShadowDepth, "direct-shadow-depth");
		INIT_SHADER_VFG(PointShadowDepth, "point-shadow-depth");
		INIT_SHADER_VF(HDR, "hdr");

#undef INIT_SHADER_VF
#undef INIT_SHADER_VFG
	}
}