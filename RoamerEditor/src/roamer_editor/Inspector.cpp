#include "roamer_editor/Inspector.hpp"
#include "roamer_editor/Editor.hpp"
#include "roamer_editor/CameraEditor.hpp"
#include "roamer_editor/LightEditor.hpp"
#include "roamer_editor/RendererEditor.hpp"
#include "roamer_editor/TransformEditor.hpp"
#include <roamer_engine/display/DisplayObject.hpp>

namespace qy::cg::editor {

	struct Inspector::Impl {
		std::weak_ptr<DisplayObject> inspectedObject;
		ptr_vector<Editor> editors;
	};

	DEFINE_OBJECT(Inspector);

	Inspector* Inspector::instance() {
		static Inspector _instance;
		return &_instance;
	}

	void Inspector::inspect(const ptr<DisplayObject>& obj) {
		if (pImpl->inspectedObject.lock() == obj) return;
		pImpl->inspectedObject = obj;
		pImpl->editors.clear();
		for (auto&& comp : obj->getComponents()) {
			auto name = NAMEOF_SHORT_TYPE_RTTI(*comp);
			auto editor = EditorFactory::create(name);
			if (!editor) editor = std::make_unique<Editor>();
			editor->target = comp;
			pImpl->editors.emplace_back(std::move(editor));
		}
		std::ranges::for_each(pImpl->editors, &Editor::start);
	}

	ptr<DisplayObject> Inspector::inspectedObject() const {
		return pImpl->inspectedObject.lock();
	}

	void Inspector::onGUI() {
		if (pImpl->inspectedObject.expired())
			return;
		auto obj = pImpl->inspectedObject.lock();
		obj->setActive(CheckBox("##Active", obj->activeSelf()));
		ImGui::SameLine();
		obj->name(InputText("##Name", obj->name()));
		const int flags = ImGuiTreeNodeFlags_DefaultOpen;
		for (auto&& e : pImpl->editors) {
			bool tree = ImGui::TreeNodeEx(e.get(), flags, NAMEOF_SHORT_TYPE_RTTI(*e->target.lock().get()).c_str());
			ImGui::SameLine(ImGui::GetWindowWidth() - 50);
			auto&& target = e->target.lock();
			target->enabled(CheckBox("##Enabled", target->enabled()));
			if (tree) {
				e->onInspectorGUI();
				ImGui::TreePop();
			}
		}
	}
}