#include "roamer_editor/TransformEditor.hpp"
#include <roamer_engine/display/Transform.hpp>

namespace qy::cg::editor {

	void TransformEditor::onInspectorGUI() {
		auto&& transform = lockTarget<Transform>();

		transform->position(DragFloat3("Position", transform->position(), 0.1f));
		transform->scale(DragFloat3("Scale", transform->scale(), 0.01f));
		transform->rotation(DragFloat3("Rotation", glm::eulerAngles(transform->rotation()), 0.01f));
	}
}