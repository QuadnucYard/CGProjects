#pragma once
#include "Material.hpp"

namespace qy::cg {

	class Materials {

	public:
		
		inline static ptr<Material> GeomUnlit;
		inline static ptr<Material> Skybox;
		inline static ptr<Material> Lit;

		static void __init__();
	};
}