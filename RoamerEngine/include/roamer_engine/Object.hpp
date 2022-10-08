#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <algorithm>
#include <ranges>
#include <type_traits>

namespace qy::cg {
	class Object {
	public:
		Object() = default;
		Object(const Object& other) = delete;
	};

	template <class T>
	using impl_ptr = std::unique_ptr<T>;

	template <class T, class U>
	inline constexpr bool isinstance(const std::shared_ptr<U>& x) {
		return (bool)std::dynamic_pointer_cast<T>(x);
		//return std::is_constructible_v<typename std::shared_ptr<U>::element_type, T>;
	}

	template <class T, class U>
	inline constexpr bool isinstance(std::shared_ptr<U>&& x) {
		return (bool)std::dynamic_pointer_cast<T>(x);
	}

	template <class T>
	inline auto instantiate() {
		return std::make_shared<T>();
	}

#define DECL_OBJECT(class_) \
	class_(); \
	~class_();

#define DECL_PIMPL \
	struct Impl; \
	impl_ptr<Impl> pImpl;

#define DEFINE_OBJECT(class_) \
	class_::class_() : pImpl(std::make_unique<class_::Impl>()) {} \
	class_::~class_() = default;

}