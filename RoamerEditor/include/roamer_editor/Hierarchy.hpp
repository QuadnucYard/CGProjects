#pragma once
#include "EditorGUI.hpp"

namespace qy::cg::editor {

	class Hierarchy : public EditorGUI {

	public:
		static Hierarchy* instance();
		void onGUI();
	};

}