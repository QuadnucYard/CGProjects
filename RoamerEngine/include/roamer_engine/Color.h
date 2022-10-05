#pragma once
#include <glm/glm.hpp>

namespace qy::cg {
	class Color {
	public:
		
		/// <summary>
		/// Convert HSV to RGB.
		/// </summary>
		/// <param name="h">Hue</param>
		/// <param name="s">Saturation</param>
		/// <param name="v">Value</param>
		/// <param name="a">Alpha</param>
		/// <returns>RGBA</returns>
		static glm::vec4 hsv2rgb(float h, float s, float v, float a = 1) {
			if (s == 0) {
				return {v, v, v, a};
			} else {
				h = std::fmod(std::fmod(h, 1) + 1, 1);
				int i = h * 6;
				float c = v * s;
				float x = c * (1 - std::abs(std::fmod(h * 6, 2) - 1));
				glm::vec4 t {v - c, v - c, v - c, a};
				switch (i) {
					case 0: return t + glm::vec4 {c, x, 0, 0};
					case 1: return t + glm::vec4 {x, c, 0, 0};
					case 2: return t + glm::vec4 {0, c, x, 0};
					case 3: return t + glm::vec4 {0, x, c, 0};
					case 4: return t + glm::vec4 {x, 0, c, 0};
					case 5: return t + glm::vec4 {c, 0, x, 0};
				}
			}
		}

		/// <summary>
		/// Convert HSV to RGB.
		/// </summary>
		/// <param name="c">Color of HSVA</param>
		/// <returns>RGBA</returns>
		static glm::vec4 hsv2rgb(glm::vec4 c) {
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
		static glm::vec4 rgb2hsv(float r, float g, float b, float a = 1) {
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
		static glm::vec4 rgb2hsv(glm::vec4 c) {
			return rgb2hsv(c.r, c.g, c.b, c.a);
		}
	};

}