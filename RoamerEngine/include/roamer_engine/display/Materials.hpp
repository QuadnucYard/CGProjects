#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class Material;

	class Materials {

	public:
		
		inline static ptr<Material> Unlit;
		inline static ptr<Material> Skybox;
		inline static ptr<Material> Lit;

		static void __init__();
	};
}