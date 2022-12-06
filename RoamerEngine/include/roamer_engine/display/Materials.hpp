#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class Material;

	class Materials {

	public:
		
		inline static ptr<Material> Unlit;
		inline static ptr<Material> Lit;
		inline static ptr<Material> SpriteUnlit;
		inline static ptr<Material> SpriteLit;
		inline static ptr<Material> Skybox;

		static void __init__();
	};
}