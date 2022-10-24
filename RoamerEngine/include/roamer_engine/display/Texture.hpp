#pragma once
#include "../Object.hpp"

namespace qy::cg {

	enum class TextureWrapMode {
		Repeat = GL_REPEAT,
		Clamp = GL_CLAMP_TO_BORDER,
		ClampEdge = GL_CLAMP_TO_EDGE,
		Mirror = GL_MIRRORED_REPEAT,
		MirrorOnce = GL_MIRROR_CLAMP_TO_EDGE,
	};
	

	enum class TextureFilterMode {
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
	};


	class Texture: public Object {
	public:
		DECL_OBJECT(Texture);

		virtual void __() {}

		GLuint id() const;
		int width() const;
		int height() const;

		TextureFilterMode getFilterMode() const;
		void setFilterMode(TextureFilterMode value);
		TextureWrapMode getWrapMode() const;
		void setWrapMode(TextureWrapMode value);
		TextureWrapMode getWrapModeU() const;
		void setWrapModeU(TextureWrapMode value);
		TextureWrapMode getWrapModeV() const;
		void setWrapModeV(TextureWrapMode value);

		static ptr<Texture> loadFromFile(const fs::path& path);

	protected:
		DECL_PIMPL;
	};
}