#pragma once
#include "Texture.hpp"

namespace qy::cg {

	class Texture2D: public Texture {
	public:
		DECL_OBJECT(Texture2D);

		//static ptr<Texture2D> createExternalTexture();
		static ptr<Texture2D> whiteTexture();

	private:
		static ptr<Texture2D> createOnePixelTexture(const color_t& color);

		DECL_PIMPL;
	};

}