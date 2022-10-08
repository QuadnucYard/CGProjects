#pragma once
#include <random>
#include <ctime>
#include <type_traits>

namespace qy::cg {

	class Random {
	private:
		inline static std::default_random_engine rng {(unsigned)time(nullptr)};

	public:
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
	};
}