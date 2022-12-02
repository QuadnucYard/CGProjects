#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Transform.hpp>

namespace qy::cg::editor {

	class TransformEditor : public Editor<Transform> {

	public:
		using Editor::Editor;

		void start() override;

		void onInspectorGUI() override;

	private:
		glm::vec3 eulerAngles;
	};

}