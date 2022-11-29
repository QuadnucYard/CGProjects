﻿#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Camera.hpp>

namespace qy::cg::editor {

	class CameraEditor: public Editor<Camera> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override;
	};

}