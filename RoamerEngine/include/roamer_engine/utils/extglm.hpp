#pragma once
#include <glm/glm.hpp>
#include <numbers>

namespace glm::ext {

	inline constexpr vec3 vec3_right {1, 0, 0};
	inline constexpr vec3 vec3_left {-1, 0, 0};
	inline constexpr vec3 vec3_up {0, 1, 0};
	inline constexpr vec3 vec3_down {0, -1, 0};
	inline constexpr vec3 vec3_forward {0, 0, 1};
	inline constexpr vec3 vec3_back {0, 0, -1};

	inline constexpr vec3 vec3_zero {0, 0, 0};
	inline constexpr vec3 vec3_one {1, 1, 1};

	inline constexpr vec3 vec3_neginf {std::numeric_limits<float>::min()};
	inline constexpr vec3 vec3_posinf {std::numeric_limits<float>::max()};

	inline constexpr float operator""_deg(long double degree) {
		return static_cast<float>(glm::radians(degree));
	}

	inline constexpr float operator""_rad(long double radian) {
		return static_cast<float>(glm::degrees(radian));
	}

}

namespace glm {
	using namespace ext;
}

namespace qy::cg {
	using namespace glm::ext;
}