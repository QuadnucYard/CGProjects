#pragma once
#include "imgui.hpp"
#include <roamer_engine/recfg.hpp>
#include <magic_enum.hpp>
#include <format>

namespace qy::cg::editor {

	class EditorGUI {

	public:
		static color_t ColorEdit4(const char* label, color_t col, ImGuiColorEditFlags flags = 0) {
			ImGui::ColorEdit4(label, glm::value_ptr(col), flags);
			return col;
		}

		static float DragFloat(const char* label, float v, float v_speed = 1) {
			auto x = __autoFormat(v_speed);
			ImGui::DragFloat(label, &v, v_speed, 0.0f, 0.0f, __autoFormat(v_speed));
			return v;
		}

		static vec2 DragFloat2(const char* label, vec2 v, float v_speed = 1) {
			ImGui::DragFloat2(label, glm::value_ptr(v), v_speed, 0.0f, 0.0f, __autoFormat(v_speed));
			return v;
		}

		static vec3 DragFloat3(const char* label, vec3 v, float v_speed = 1) {
			ImGui::DragFloat3(label, glm::value_ptr(v), v_speed, 0.0f, 0.0f, __autoFormat(v_speed));
			return v;
		}

		static bool CheckBox(const char* label, bool v) {
			ImGui::Checkbox(label, &v);
			return v;
		}

		static std::string InputText(const char* label, std::string s) {
			ImGui::InputText(label, &s);
			return std::move(s);
		}

		template <typename E>
		static E ComboEnum(const char* label, E e) {
			E result {e};
			if (ImGui::BeginCombo(label, magic_enum::enum_name(e).data())) {
				for (auto&& [t, l] : magic_enum::enum_entries<E>()) {
					bool is_selected = (e == t);
					if (ImGui::Selectable(l.data(), is_selected))
						result = t;
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			return result;
		}

		template <typename E>
		static void ComboEnum(const char* label, E e, auto&& onSelected) {
			if (ImGui::BeginCombo(label, magic_enum::enum_name(e).data())) {
				for (auto&& [t, l] : magic_enum::enum_entries<E>()) {
					bool is_selected = (e == t);
					if (ImGui::Selectable(l.data(), is_selected))
						onSelected(t);
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
		}

	private:
		static const char* __autoFormat(float v) {
			int e = (int)-std::log10(v);
			if (e == 3) return "%.3f";
			if (e == 2) return "%.2f";
			if (e == 1) return "%.1f";
			if (e == 0) return "%.0f";
			return "%.3f";
		}
	};
}