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
		ptr_vector<EditorBase> editors;
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
			auto name = NAMEOF_TYPE_RTTI(*comp);
			/*auto editor = ObjectFactory::create(name + "Editor");
			if (editor) {
				auto editor2 = dynamic_cast<EditorBase*>(editor);
				editor2->setTarget(comp); // 这里反射做不到了
				pImpl->editors.emplace_back(editor2);
			}*/
			if (isinstance<Transform>(comp)) { auto e = instantiate<TransformEditor>(); e->setTarget(comp); pImpl->editors.push_back(std::move(e)); }
			if (isinstance<Camera>(comp)) { auto e = instantiate<CameraEditor>(); e->setTarget(comp); pImpl->editors.push_back(std::move(e)); }
			if (isinstance<Light>(comp)) { auto e = instantiate<LightEditor>(); e->setTarget(comp); pImpl->editors.push_back(std::move(e)); }
			if (isinstance<Renderer>(comp)) { auto e = instantiate<RendererEditor>(); e->setTarget(comp); pImpl->editors.push_back(std::move(e)); }
		}
		std::ranges::for_each(pImpl->editors, &EditorBase::start);
	}

	ptr<DisplayObject> Inspector::inspectedObject() const {
		return pImpl->inspectedObject.lock();
	}

	void Inspector::onGUI() {
		auto const disqualifiedName = [](std::string_view name) {
			if (size_t p = name.rfind(':'); p != std::string_view::npos) {
				return name.substr(p + 1);
			} else {
				return name;
			}
		};

		if (!pImpl->inspectedObject.expired()) {
			auto obj = pImpl->inspectedObject.lock();
			obj->setActive(CheckBox("##Active", obj->activeSelf()));
			ImGui::SameLine();
			obj->name(InputText("##Name", obj->name()));
			const int flags = ImGuiTreeNodeFlags_DefaultOpen;
			for (auto&& comp : obj->getComponents()) {
				bool tree = ImGui::TreeNodeEx(comp.get(), flags, disqualifiedName(typeid(*comp).name()).data());
				ImGui::SameLine(ImGui::GetWindowWidth() - 50);
				comp->enabled(CheckBox("##Enabled", comp->enabled()));
				
				if (tree) {
					std::ranges::for_each(pImpl->editors, &EditorBase::onInspectorGUI);
					//auto n = typeid(decltype(*comp)).name();
					//不知道为啥nameof能正确rtti  但没啥用  还是不能反射
					ImGui::TreePop();
				}
				
			}
		}
	}
}