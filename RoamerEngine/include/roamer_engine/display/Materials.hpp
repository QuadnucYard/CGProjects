#pragma once
#include "Material.hpp"

namespace qy::cg {

	class Materials {

	public:
		
		inline static ptr<Material> geom_unlit;
		inline static ptr<Material> skybox;

		static void __init__();
	};
}