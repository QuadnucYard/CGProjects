#pragma once
#include "Editor.hpp"

namespace qy::cg::editor {

	class CameraEditor: public Editor {
		RegisterEditor(CameraEditor, Camera);
	public:
		void onInspectorGUI() override;
	};

}