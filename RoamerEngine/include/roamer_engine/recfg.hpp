#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "utils/extglm.hpp"

#include <filesystem>
#include <numeric>
#include <ranges>
#include <type_traits>

namespace qy::cg {

	namespace fs = std::filesystem;

	template <class T>
	using ptr = std::shared_ptr<T>;

	template <class T>
	using impl_ptr = std::unique_ptr<T>;

	template <class T>
	using ptr_vector = std::vector<ptr<T>>;

	using color_t = glm::vec4;
	using glm::vec2, glm::vec3, glm::vec4, glm::mat3, glm::mat4, glm::quat;

	template <class T, class U>
	inline constexpr bool isinstance(const ptr<U>& x) {
		return (bool)std::dynamic_pointer_cast<T>(x);
		//return std::is_constructible_v<typename std::shared_ptr<U>::element_type, T>;
	}

	template <class T, class U>
	inline constexpr bool isinstance(ptr<U>&& x) {
		return (bool)std::dynamic_pointer_cast<T>(x);
	}

	template <class T>
	inline auto instantiate() {
		return std::make_shared<T>();
	}

	template <class T>
	inline auto enum_cast(T value) {
		return static_cast<std::underlying_type_t<T>>(value);
	}

}