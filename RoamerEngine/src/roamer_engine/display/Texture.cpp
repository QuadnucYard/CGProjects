#include "roamer_engine/display/Texture.hpp"
#include "roamer_engine/display/Texture2D.hpp"

namespace qy::cg {

	struct Texture::Impl {
		GLuint texture;
		int width;
		int height;
		TextureFilterMode filterMode;
		TextureWrapMode wrapModeU;
		TextureWrapMode wrapModeV;

		Impl(): width(0), height(0) {
			glGenTextures(1, &texture);
			rebind();
			setFilterMode(TextureFilterMode::Nearest);
			setWrapModeU(TextureWrapMode::Repeat);
			setWrapModeV(TextureWrapMode::Repeat);
		}

		~Impl() {
			glDeleteTextures(1, &texture);
		}

		Impl* rebind() {
			glBindTexture(GL_TEXTURE_2D, texture);
			return this;
		}

		void setFilterMode(TextureFilterMode value) {
			filterMode = value;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, enum_cast(value));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, enum_cast(value));
		}

		void setWrapModeU(TextureWrapMode value) {
			wrapModeU = value;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, enum_cast(value));
		}

		void setWrapModeV(TextureWrapMode value) {
			wrapModeV = value;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, enum_cast(value));
		}
	};

	DEFINE_OBJECT(Texture);

	GLuint Texture::id() const { return pImpl->texture; }

	int Texture::width() const { return pImpl->width; }
	int Texture::height() const { return pImpl->height; }

	TextureFilterMode Texture::getFilterMode() const { return pImpl->filterMode; }
	void Texture::setFilterMode(TextureFilterMode value) { pImpl->rebind()->setFilterMode(value); }
	TextureWrapMode Texture::getWrapMode() const { return pImpl->wrapModeU; }
	void Texture::setWrapMode(TextureWrapMode value) { pImpl->rebind()->setWrapModeU(value); pImpl->setWrapModeV(value); }
	TextureWrapMode Texture::getWrapModeU() const { return pImpl->wrapModeU; }
	void Texture::setWrapModeU(TextureWrapMode value) { pImpl->rebind()->setWrapModeU(value); }
	TextureWrapMode Texture::getWrapModeV() const { return pImpl->wrapModeU; }
	void Texture::setWrapModeV(TextureWrapMode value) { pImpl->rebind()->setWrapModeV(value); }

	void Texture::__setSize(int width, int height) {
		pImpl->width = width;
		pImpl->height = height;
	}

}