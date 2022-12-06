#pragma once
#include "../Object.hpp"
#include "../Rect.hpp"

namespace qy::cg {

	class Texture2D;

	class Sprite : public Object {

	public:
		DECL_OBJECT(Sprite);

		ptr<Texture2D> texture() const;

		static ptr<Sprite> create(ptr<Texture2D> texture, rectf rect, vec2 pivot, float pixelsPerUnit);

		void __render();

	private:
		friend class SpriteRenderer;
		DECL_PIMPL;
	};
}