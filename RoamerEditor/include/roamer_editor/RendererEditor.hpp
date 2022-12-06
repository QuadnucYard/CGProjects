#pragma once
#include "Editor.hpp"

namespace qy::cg::editor {

	class RendererEditor: public Editor {
		RegisterEditor(RendererEditor, Renderer);
		RegisterEditor(RendererEditor, MeshRenderer);
		RegisterEditor(RendererEditor, SpriteRenderer);
	public:
		void onInspectorGUI() override;
	};

}