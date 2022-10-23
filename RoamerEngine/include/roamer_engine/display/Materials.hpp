#pragma once
#include "Material.hpp"
#include "Shaders.hpp"

namespace qy::cg {

	class Materials {

	public:
		
		inline static ptr<Material> geom_unlit;

		static void __init__() {
			geom_unlit = instantiate<Material>();
			geom_unlit->setShader(Shaders::geom_unlit);
		}
	};
}