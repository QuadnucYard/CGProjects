#include "roamer_editor/TransformEditor.hpp"
#include <roamer_engine/display/Transform.hpp>

namespace qy::cg::editor {

	void TransformEditor::onInspectorGUI() {
		auto&& transform = lockTarget<Transform>();

		DragFloat3("Position", transform->position(), 0.1f);
		DragFloat3("Scale", transform->scale(), 0.01f);
		DragFloat3("Rotation", glm::eulerAngles(transform->rotation()), 0.01f);
	}
}