#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class DisplayObject;

	class ModelLoader {

	public:
		static ptr<DisplayObject> loadModel(const fs::path& path);

	private:
		struct Impl;
	};
}