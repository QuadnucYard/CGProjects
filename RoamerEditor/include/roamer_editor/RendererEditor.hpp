#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Renderer.hpp>

namespace qy::cg::editor {

	class RendererEditor: public Editor<Renderer> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override;

	};

}