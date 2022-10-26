#include "roamer_engine/display/Texture2D.hpp"

namespace qy::cg {

	struct Texture2D::Impl {

	};

	Texture2D::Texture2D(): Texture(), pImpl(std::make_unique<Texture2D::Impl>()) {}
	Texture2D::~Texture2D() = default;

	ptr<Texture2D> Texture2D::whiteTexture() {
		static ptr<Texture2D> tex = createOnePixelTexture({1, 1, 1, 1});
		return tex;
	}

	ptr<Texture2D> Texture2D::createOnePixelTexture(const color_t& color) {
		auto tex = instantiate<Texture2D>();
		char color32[] {color.r * 255, color.g * 255, color.b * 255, color.a * 255};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color32);
		glGenerateMipmap(GL_TEXTURE_2D);

		return tex;
	}

}
