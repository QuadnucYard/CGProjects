#pragma once
#include "Object.hpp"
#include "display/Texture2D.hpp"

namespace qy::cg {

	class Assets {
	public:
		template <class T>
		static ptr<T> load(const fs::path& path);
	};

	template<>
	ptr<Texture2D> Assets::load(const fs::path& path) {
		return Texture2D::load(path);
	}
}