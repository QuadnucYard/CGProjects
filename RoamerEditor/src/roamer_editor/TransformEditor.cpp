#include "roamer_editor/TransformEditor.hpp"
#include <roamer_engine/display/Transform.hpp>
#include <iostream>

namespace qy::cg::editor {

	void TransformEditor::onInspectorGUI() {
		auto&& transform = lockTarget<Transform>();

		transform->position(DragFloat3("Position", transform->position(), 0.1f));
		transform->scale(DragFloat3("Scale", transform->scale(), 0.01f));
		auto curRotation = transform->rotation();
		if (glm::length(curRotation - prevRotation) > 1e-6)
			eulerAngles = glm::degrees(glm::eulerAngles(curRotation));
		eulerAngles = DragFloat3("Rotation", eulerAngles);
		transform->rotation(prevRotation = glm::radians(eulerAngles));
	}
}