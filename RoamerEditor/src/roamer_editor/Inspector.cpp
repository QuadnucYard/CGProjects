#include "roamer_editor/Inspector.hpp"
#include "roamer_editor/Editor.hpp"
#include "roamer_editor/CameraEditor.hpp"
#include "roamer_editor/LightEditor.hpp"
#include "roamer_editor/TransformEditor.hpp"
#include <roamer_engine/display/DisplayObject.hpp>

namespace qy::cg::editor {

	void Inspector::onGUI() {
		auto const disqualifiedName = [](std::string_view name) {
			if (size_t p = name.rfind(':'); p != std::string_view::npos) {
				return name.substr(p + 1);
			} else {
				return name;
			}
		};

		if (!inspectedObject.expired()) {
			auto obj = inspectedObject.lock();
			obj->setActive(CheckBox("##Active", obj->activeSelf()));
			ImGui::SameLine();
			obj->name(InputText("##Name", obj->name()));
			const int flags = ImGuiTreeNodeFlags_DefaultOpen;
			for (auto&& comp : obj->getComponents()) {
				bool tree = ImGui::TreeNodeEx(comp.get(), flags, disqualifiedName(typeid(*comp).name()).data());
				ImGui::SameLine(ImGui::GetWindowWidth() - 40);
				comp->enabled(CheckBox("##Enabled", comp->enabled()));
				if (tree) {
					if (isinstance<Transform>(comp)) TransformEditor(comp).onInspectorGUI();
					if (isinstance<Camera>(comp)) CameraEditor(comp).onInspectorGUI();
					if (isinstance<Light>(comp)) LightEditor(comp).onInspectorGUI();
					//auto n = typeid(decltype(*comp)).name();
					//不知道为啥nameof能正确rtti  但没啥用  还是不能反射
					ImGui::TreePop();
				}
			}
		}
	}
}