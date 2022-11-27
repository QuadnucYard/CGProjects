#pragma once
#include "EditorGUI.hpp"
#include <memory>

namespace qy::cg {
	class DisplayObject;
}

namespace qy::cg::editor {

	class Inspector : public EditorGUI {

	public:
		static void onGUI();

	public:
		inline static std::weak_ptr<DisplayObject> inspectedObject;
	};

}