#pragma once
#include "imgui.hpp"
#include <memory>

namespace qy::cg {
	class DisplayObject;
}

namespace qy::cg::editor {

	class Inspector {

	public:
		static void onGUI();

	public:
		inline static std::weak_ptr<DisplayObject> inspectedObject;
	};

}