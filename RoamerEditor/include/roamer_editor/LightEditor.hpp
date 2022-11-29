#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Light.hpp>

namespace qy::cg::editor {

	class LightEditor: public Editor<Light> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override;
	};

}