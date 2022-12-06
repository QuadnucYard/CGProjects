#include "roamer_editor/Hierarchy.hpp"
#include "roamer_editor/Inspector.hpp"
#include <roamer_engine/display/Transform.hpp>
#include <roamer_engine/display/Scene.hpp>
#include <roamer_engine/display/DisplayObject.hpp>

namespace qy::cg::editor {

	Hierarchy* Hierarchy::instance() {
		static Hierarchy _instance;
		return &_instance;
	}

	void Hierarchy::onGUI() {

		const auto dfs = [&](this auto&& self, const TransformPtr& parent) -> void {
			for (auto&& child : parent) {
				int flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
				if (child->childCount() == 0) 
					flags |= ImGuiTreeNodeFlags_Leaf;
				if (child->obj() == Inspector::instance()->inspectedObject())
					flags |= ImGuiTreeNodeFlags_Selected;				
				with_TreeNodeEx(child.get(), flags, child->obj()->name().c_str()) {
					// 问题在于，上一个item不是这个结点
					if (ImGui::IsItemClicked()) {
						Inspector::instance()->inspect(child->obj());
					}
					self(child);
				}
			}
		};
		dfs(Scene::current()->root());
	}
}