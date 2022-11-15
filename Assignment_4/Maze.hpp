#pragma once
#include <time.h>
#include<vector>

//方法来自https://github.com/victorlaurentiu/MazeGenerator_graphsUnioning
namespace qy::cg {
	class Maze
	{
	private:
		std::vector<std::vector<int>> maze;
		struct vec2
		{
			int x, y;
			vec2() {}
			vec2(int x, int y) : x(x), y(y) {}
			// distance^2
			int distance2(const vec2& other) const
			{
				const int dx = (other.x - x);
				const int dy = (other.y - y);
				return dx * dx + dy * dy;
			}
			vec2& operator= (const vec2& other)
			{
				if (this == &other)
					return *this;

				x = other.x;
				y = other.y;

				return *this;
			}
			const vec2 operator+ (const vec2& other) const { return vec2{ x + other.x, y + other.y }; }
			vec2 operator/ (int devider) const { return vec2{ x / devider, y / devider }; }
			bool operator== (const vec2& other) const { return (x == other.x) && (y == other.y); }
			bool operator!= (const vec2& other) const { return !(*this == other); }
		};
		int width, height;
	public:
		Maze(int width, int height);
		bool rand_true(int nr) { return ((float)rand() / RAND_MAX < (float)1 / nr); }
		std::vector<std::vector<int>> getMaze() { return maze; }
	};
}