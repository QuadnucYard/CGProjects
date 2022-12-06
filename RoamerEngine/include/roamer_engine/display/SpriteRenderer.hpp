#pragma once
#include "Renderer.hpp"

namespace qy::cg {

	class Sprite;

	class SpriteRenderer: public Renderer {

	public:
		DECL_OBJECT(SpriteRenderer);

		void __render() override;

		ptr<Sprite> getSprite() const;
		void setSprite(ptr<Sprite> value);

	private:
		DECL_PIMPL;
	};
}