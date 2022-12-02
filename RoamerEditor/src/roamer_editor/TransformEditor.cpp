#include "roamer_editor/TransformEditor.hpp"
#include <roamer_engine/display/Transform.hpp>

namespace qy::cg::editor {

	void TransformEditor::onInspectorGUI() {
		auto&& transform = lockTarget<Transform>();

		transform->position(DragFloat3("Position", transform->position(), 0.1f));
		transform->scale(DragFloat3("Scale", transform->scale(), 0.01f));
		eulerAngles = DragFloat3("Rotation", eulerAngles);
		transform->rotation(glm::radians(eulerAngles));
	}

	void TransformEditor::start() {
		eulerAngles = glm::degrees(glm::eulerAngles(target.lock()->transform()->rotation()));
	}

}