#pragma once
#include <glm/glm.hpp>
#include <random>
#include <ctime>
#include <type_traits>

namespace qy::cg {

	class Random {
	private:
		inline static std::default_random_engine rng {(unsigned)time(nullptr)};

	public:
		static float value() {
			static std::uniform_real_distribution<float> distrib;
			return distrib(rng);
		}

		template <class T>
		static T range(T min, T max) {
			static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
			if constexpr (std::is_integral_v<T>) {
				std::uniform_int_distribution<T> distrib(min, max + 1);
				return distrib(rng);
			} else {
				std::uniform_real_distribution<T> distrib(min, max);
				return distrib(rng);
			}
		}

		template <class T>
		static T range(T max) {
			static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>);
			if constexpr (std::is_integral_v<T>) {
				std::uniform_int_distribution<T> distrib(0, max + 1);
				return distrib(rng);
			} else {
				std::uniform_real_distribution<T> distrib(0, max);
				return distrib(rng);
			}
		}

		static glm::quat rotation() {
			return glm::normalize(glm::quat {value(), value(), value(), value()});
		}

		static glm::vec2 onUnitCircle() {
			float u = value() * glm::two_pi<float>();
			return {glm::cos(u), glm::sin(u)};
		}

		static glm::vec3 onUnitSphere() {
			float u = value() * glm::two_pi<float>();
			float v = glm::acos(value() * 2 - 1);
			return {glm::cos(u) * glm::sin(v), glm::sin(u) * glm::sin(v), glm::cos(v)};
		}

		static glm::vec2 insideUnitCircle() {
			float r = std::sqrt(value());
			float u = value() * glm::two_pi<float>();
			return {r * glm::cos(u), r * glm::sin(u)};
		}

		static glm::vec3 insideUnitSphere() {
			float r = std::cbrt(value());
			float u = value() * glm::two_pi<float>();
			float v = glm::acos(value() * 2 - 1);
			return {r * glm::cos(u) * glm::sin(v), r * glm::sin(u) * glm::sin(v), r*glm::cos(v)};
		}

	};
}