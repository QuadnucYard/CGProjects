#pragma once

namespace qy::cg {

	enum class LightType {
		Directional, Point, Spot,
	};

	enum class LightShadow {
		None, Hard, Soft,
	};
}