#include "roamer_engine/display/SpriteRenderer.hpp"
#include "roamer_engine/display/Materials.hpp"
#include "roamer_engine/display/Material.hpp"
#include "roamer_engine/display/Sprite.hpp"
#include "roamer_engine/display/Texture2D.hpp"
#include "roamer_engine/display/Texture.hpp"

namespace qy::cg {

	struct SpriteRenderer::Impl {
		ptr<Sprite> sprite;
	};

	SpriteRenderer::SpriteRenderer(): Renderer(), MAKE_PIMPL {
		setSharedMaterial(Materials::SpriteUnlit);
	}

	SpriteRenderer::~SpriteRenderer() = default;

	void SpriteRenderer::__render() {
		pImpl->sprite->__render();
	}

	ptr<Sprite> qy::cg::SpriteRenderer::getSprite() const { return pImpl->sprite; }
	void qy::cg::SpriteRenderer::setSprite(ptr<Sprite> value) {
		pImpl->sprite = std::move(value);
		getMaterial()->setMainTexture(std::dynamic_pointer_cast<Texture>(pImpl->sprite->texture()));
	}

}