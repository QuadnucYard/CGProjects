#include "roamer_editor/Hierarchy.hpp"
#include "roamer_editor/Inspector.hpp"
#include <roamer_engine/display/Transform.hpp>
#include <roamer_engine/display/Scene.hpp>
#include <roamer_engine/display/DisplayObject.hpp>

namespace qy::cg::editor {

	void Hierarchy::onGUI() {

		const auto dfs = [&](this auto&& self, const TransformPtr& parent) -> void {
			for (auto&& child : parent) {
				int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
				if (child->childCount() == 0) {
					flags |= ImGuiTreeNodeFlags_Leaf;
				}
				with_TreeNodeEx(child.get(), flags, child->obj()->name().c_str()) {
					if (ImGui::IsItemClicked()) {
						Inspector::inspectedObject = child->obj();
					}
					self(child);
				}
			}
		};
		dfs(Scene::current()->root());
	}
}