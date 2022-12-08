#include "roamer_engine/display/Texture2D.hpp"
#include <SOIL2/SOIL2.h>
#include <stb_image.h>

namespace qy::cg {

	struct Texture2D::Impl {

	};

	Texture2D::Texture2D(): Texture(), MAKE_PIMPL {}
	Texture2D::~Texture2D() = default;

	ptr<Texture2D> Texture2D::whiteTexture() {
		static ptr<Texture2D> tex = createOnePixelTexture({1, 1, 1, 1});
		return tex;
	}

	ptr<Texture2D> Texture2D::load(const fs::path& path) {
		stbi_set_flip_vertically_on_load(true);

		if (!fs::exists(path)) throw std::runtime_error("Path not exist!");

		auto tex = instantiate<Texture2D>();

		int width, height, nrChannels;
		unsigned char* data = SOIL_load_image(path.string().data(), &width, &height, &nrChannels, 0);
		if (!data) throw std::runtime_error("Failed to load texture.");
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		} else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		} else if (nrChannels == 1) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		} else {
			throw std::runtime_error("Not supported image format!");
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(data);

		tex->__setSize(width, height);

		return tex;
	}

	ptr<Texture2D> Texture2D::createOnePixelTexture(const color_t& color) {
		auto tex = instantiate<Texture2D>();
		uint8_t color32[] {uint8_t(color.r * 255), uint8_t(color.g * 255), uint8_t(color.b * 255), uint8_t(color.a * 255)};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, color32);
		glGenerateMipmap(GL_TEXTURE_2D);

		return tex;
	}

}
