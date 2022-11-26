#pragma once
#include "imgui.hpp"
#include <roamer_engine/recfg.hpp>
#include <memory>

namespace qy::cg {
	class Component;
}

namespace qy::cg::editor {

	template <class T>
	class Editor {

	public:
		Editor(const std::shared_ptr<Component>& target): target(std::dynamic_pointer_cast<T>(target)) {}
		virtual void onInspectorGUI() {}

	protected:
		color_t ColorEdit4(const char* label, color_t col, ImGuiColorEditFlags flags = 0) {
			ImGui::ColorEdit4(label, glm::value_ptr(col), flags);
			return col;
		}

		float DragFloat(const char* label, float v, float v_speed = 1) {
			ImGui::DragFloat(label, &v, v_speed);
			return v;
		}

		vec2 DragFloat2(const char* label, vec2 v, float v_speed = 1) {
			ImGui::DragFloat2(label, glm::value_ptr(v), v_speed);
			return v;
		}

		vec3 DragFloat3(const char* label, vec3 v, float v_speed = 1) {
			ImGui::DragFloat3(label, glm::value_ptr(v), v_speed);
			return v;
		}

	protected:
		std::weak_ptr<T> target;
	};

}