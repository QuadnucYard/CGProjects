#include "roamer_engine/display/Materials.hpp"
#include "roamer_engine/display/Material.hpp"
#include "roamer_engine/display/Shaders.hpp"
#include "roamer_engine/display/Texture2D.hpp"

namespace qy::cg {
	void Materials::__init__()  {
		Unlit = instantiate<Material>();
		Unlit->setMainTexture(Texture2D::whiteTexture());
		Unlit->setShader(Shaders::Unlit);
		
		Lit = instantiate<Material>();
		Lit->setMainTexture(Texture2D::whiteTexture());
		Lit->setShader(Shaders::Lit);

		SpriteUnlit = Unlit->clone();

		SpriteLit = Lit->clone();

		Skybox = instantiate<Material>();
		Skybox->setMainTexture(Texture2D::whiteTexture());
		Skybox->setShader(Shaders::Skybox);

	}
}