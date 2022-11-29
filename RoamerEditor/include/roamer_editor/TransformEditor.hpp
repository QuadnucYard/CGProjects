#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Transform.hpp>

namespace qy::cg::editor {

	class TransformEditor : public Editor<Transform> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override;
	};

}