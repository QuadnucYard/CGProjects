#pragma once
#include "Object.hpp"

namespace qy::cg {

	template <typename T = float>
	class Rect {

	public:
		inline Rect() = default;
		inline Rect(T x, T y, T width, T height): m_xMin(x), m_yMin(y), m_width(width), m_height(height) {}

		inline Rect(glm::tvec2<T> position, glm::tvec2<T> size) :
			m_xMin(position.x), m_yMin(position.y), m_width(size.x), m_height(size.y) {}

		inline static Rect zero() { return Rect(0, 0, 0, 0); }

		inline static Rect minmaxRect(float xmin, float ymin, float xmax, float ymax) {
			return Rect(xmin, ymin, xmax - xmin, ymax - ymin);
		}

		inline void set(float x, float y, float width, float height) {
			m_xMin = x;
			m_yMin = y;
			m_width = width;
			m_height = height;
		}

		inline T x() const { return m_xMin; }
		inline void x(T value) { m_xMin = value; }

		inline T y() const { return m_yMin; }
		inline void y(T value) { m_yMin = value; }

		inline glm::tvec2<T> position() const { return {m_xMin, m_yMin}; }
		inline void position(glm::tvec2<T> value) { m_xMin = value.x; m_yMin = value.y; }

		inline glm::tvec2<T> center() const { return {m_xMin + m_width / 2.0f, m_yMin + m_height / 2.0f}; }
		inline void center(glm::tvec2<T> value) { m_xMin = value.x - m_width / 2.0f; m_yMin = value.y - m_height / 2.0f; }

		inline glm::tvec2<T> min() const { return {m_xMin, m_yMin}; }
		inline void min(glm::tvec2<T> value) { m_xMin = value.x; m_yMin = value.y; }

		inline glm::tvec2<T> max() const { return {xMax(), yMax()}; }
		inline void max(glm::tvec2<T> value) { xMax(value.x); yMax(value.y); }

		inline T width() const { return m_width; }
		inline void width(T value) { m_width = value; }

		inline T height() const { return m_height; }
		inline void height(T value) { m_height = value; }

		inline glm::tvec2<T> size() const { return {m_width, m_height}; }
		inline void size(glm::tvec2<T> value) { m_width = value.x; m_height = value.y; }

		inline T xMin() const { return m_xMin; }
		inline void xMin(T value) { m_width += m_xMin - value; m_xMin = value; }

		inline T yMin() const { return m_yMin; }
		inline void yMin(T value) { m_height += m_yMin - value; m_yMin = value; }

		inline T xMax() const { return m_xMin + m_width; }
		inline void xMax(T value) { m_width = value - m_xMin; }

		inline T yMax() const { return m_yMin + m_height; }
		inline void yMax(T value) { m_height = value - m_yMin; }

	private:

		T m_xMin;
		T m_yMin;
		T m_width;
		T m_height;
	};

	using rectf = Rect<float>;
	using recti = Rect<int>;
}