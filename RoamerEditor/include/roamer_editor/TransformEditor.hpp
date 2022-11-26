#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Transform.hpp>

namespace qy::cg::editor {

	class TransformEditor : public Editor<Transform> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override {
			auto&& transform = target.lock();

			transform->position(DragFloat3("Position", transform->position(), 0.1f));
			transform->scale(DragFloat3("Scale", transform->scale(), 0.01f));
			transform->rotation(glm::radians(DragFloat3("Rotation", glm::degrees(glm::eulerAngles(transform->rotation())), 0.1f)));
		}
	};

}