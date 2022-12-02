#pragma once
#include "Editor.hpp"

namespace qy::cg::editor {

	class LightEditor: public Editor {
		RegisterEditor(LightEditor, Light);
	public:
		void onInspectorGUI() override;
	};

}