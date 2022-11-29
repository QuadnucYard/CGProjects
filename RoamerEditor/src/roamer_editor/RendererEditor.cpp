#include "roamer_editor/RendererEditor.hpp"
#include <roamer_engine/display/Material.hpp>

namespace qy::cg::editor {

	void RendererEditor::onInspectorGUI() {
		auto&& renderer = target.lock();
		ImGui::Text("Materials");
		ImGui::SameLine();
		if (ImGui::Button("Make exclusive")) {
			renderer->getMaterials();
		}
		auto&& sharedMaterials = renderer->getSharedMaterials();
		for (int i = 0; auto && mat : renderer->__getMaterials()) {
			with_TreeNodeEx(mat.get(), 0, std::format("{}{}", i, mat == sharedMaterials[i] ? "*" : "").c_str()) {
				for (auto&& [k, v] : mat->getInts()) {
					mat->setInt(k, DragInt(k, v));
				}
				for (auto&& [k, v] : mat->getFloats()) {
					mat->setFloat(k, DragFloat(k, v, 0.1f));
				}
				for (auto&& [k, v] : mat->getColors()) {
					mat->setColor(k, ColorEdit4(k, v));
				}
			}
			i++;
		}
	}

}