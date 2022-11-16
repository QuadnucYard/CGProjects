#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class DisplayObject;

	class ModelLoader {

	public:
		static ptr<DisplayObject> loadObj(const fs::path& path);
	};
}