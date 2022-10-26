#include "roamer_engine/display/Materials.hpp"
#include "roamer_engine/display/Shaders.hpp"
#include "roamer_engine/display/Texture2D.hpp"

namespace qy::cg {
	void Materials::__init__()  {
		geom_unlit = instantiate<Material>();
		geom_unlit->setMainTexture(Texture2D::whiteTexture());
		geom_unlit->setShader(Shaders::geom_unlit);
		skybox = instantiate<Material>();
		skybox->setMainTexture(Texture2D::whiteTexture());
		skybox->setShader(Shaders::skybox);
	}
}