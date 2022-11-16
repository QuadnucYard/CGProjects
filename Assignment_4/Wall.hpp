#pragma once
#include<roamer_engine/display.hpp>

namespace qy::cg {
	class Wall {
	private:
		ptr<DisplayObject> wallObj;
	public:
		Wall();
		std::shared_ptr<DisplayObject> getObj() { return wallObj; }
		void scale(const glm::vec3& value);
		void position(const glm::vec3& value) { wallObj->transform()->position(value); }
		void rotation(const glm::vec3& value) { wallObj->transform()->rotation(value); }
	};
}