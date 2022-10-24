#pragma once
#include "Texture.hpp"

namespace qy::cg {

	class Texture2D: public Texture {
	public:
		DECL_OBJECT(Texture2D);

		//static ptr<Texture2D> createExternalTexture();

	private:
		DECL_PIMPL;
	};

}