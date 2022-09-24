#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <algorithm>
#include <ranges>
#include <type_traits>

namespace qy::cg {
	class Object {
	};

	template <class T, class U>
	inline constexpr bool isinstance(const std::shared_ptr<U>& x) {
		return (bool)std::dynamic_pointer_cast<T>(x);
		//return std::is_constructible_v<typename std::shared_ptr<U>::element_type, T>;
	}

	template <class T, class U>
	inline constexpr bool isinstance(std::shared_ptr<U>&& x) {
		return (bool)std::dynamic_pointer_cast<T>(x);
	}

}