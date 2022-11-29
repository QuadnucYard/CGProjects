#include "roamer_editor/TransformEditor.hpp"

namespace qy::cg::editor {

	void TransformEditor::onInspectorGUI() {
		auto&& transform = target.lock();

		transform->position(DragFloat3("Position", transform->position(), 0.1f));
		transform->scale(DragFloat3("Scale", transform->scale(), 0.01f));
		transform->rotation(glm::radians(DragFloat3("Rotation", glm::degrees(glm::eulerAngles(transform->rotation())), 1.0f)));
	}

}