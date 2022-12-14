#pragma once
#include <glm/glm.hpp>

namespace qy::cg {
	class Color {
	public:

		/// <summary>
		/// Convert HSV to RGB.
		/// </summary>
		/// <param name=.hpp">Hue</param>
		/// <param name="s">Saturation</param>
		/// <param name="v">Value</param>
		/// <param name="a">Alpha</param>
		/// <returns>RGBA</returns>
		static color_t hsv2rgb(float h, float s, float v, float a = 1) {
			if (s != 0) {
				h = std::fmod(std::fmod(h, 1.0f) + 1.0f, 1.0f);
				int i = int(h * 6);
				float c = v * s;
				float x = c * (1 - std::abs(std::fmod(h * 6, 2.0f) - 1));
				color_t t {v - c, v - c, v - c, a};
				switch (i) {
					case 0: return t + color_t {c, x, 0, 0};
					case 1: return t + color_t {x, c, 0, 0};
					case 2: return t + color_t {0, c, x, 0};
					case 3: return t + color_t {0, x, c, 0};
					case 4: return t + color_t {x, 0, c, 0};
					case 5: return t + color_t {c, 0, x, 0};
				}
			}
			return {v, v, v, a};
		}

		/// <summary>
		/// Convert HSV to RGB.
		/// </summary>
		/// <param name="c">Color of HSVA</param>
		/// <returns>RGBA</returns>
		static color_t hsv2rgb(color_t c) {
			return hsv2rgb(c.r, c.g, c.b, c.a);
		}

		/// <summary>
		/// Convert RGB to HSV.
		/// </summary>
		/// <param name="r">Red</param>
		/// <param name="g">Green</param>
		/// <param name="b">Blue</param>
		/// <param name="a">Alpha</param>
		/// <returns>HSVA</returns>
		static color_t rgb2hsv(float r, float g, float b, float a = 1) {
			float max = std::max(std::max(r, g), b), min = std::min(std::min(r, g), b), dif = max - min;
			float h = 0;
			if (max == min) h = 0;
			else if (min == b) h = (g - r) / dif + 1;
			else if (min == r) h = (b - g) / dif + 3;
			else if (min == g) h = (r - b) / dif + 5;
			float s = max == 0 ? 0 : 1 - min / max;
			return {h / 6.0f, s, max, a};
		}

		/// <summary>
		/// Convert RGB to HSV.
		/// </summary>
		/// <param name="c">Color of RGBA</param>
		/// <returns>HSVA</returns>
		static color_t rgb2hsv(color_t c) {
			return rgb2hsv(c.r, c.g, c.b, c.a);
		}

		static color_t rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
			return {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};
		}
	};
}