#pragma once
#include "Editor.hpp"

namespace qy::cg::editor {

	class TransformEditor : public Editor {
		RegisterEditor(TransformEditor, Transform);
	public:
		void start() override;
		void onInspectorGUI() override;
	private:
		glm::vec3 eulerAngles;
	};

}