#pragma once
#include "imgui.hpp"
#include <roamer_engine/Object.hpp>
#include <magic_enum.hpp>
#include <format>

namespace qy::cg::editor {

	class EditorGUI : public Object {

	public:
		static color_t ColorEdit4(std::string_view label, color_t col, ImGuiColorEditFlags flags = 0) {
			ImGui::ColorEdit4(label.data(), glm::value_ptr(col), flags);
			return col;
		}

		static int DragInt(std::string_view label, int v, float v_speed = 1) {
			ImGui::DragInt(label.data(), &v, v_speed);
			return v;
		}

		static vec2 DragInt2(std::string_view label, glm::ivec2 v, float v_speed = 1) {
			ImGui::DragInt2(label.data(), glm::value_ptr(v), v_speed);
			return v;
		}

		static vec3 DragInt3(std::string_view label, glm::ivec3 v, float v_speed = 1) {
			ImGui::DragInt3(label.data(), glm::value_ptr(v), v_speed);
			return v;
		}

		static float DragFloat(std::string_view label, float v, float v_speed = 1) {
			ImGui::DragFloat(label.data(), &v, v_speed, 0.0f, 0.0f, __autoFormat(v_speed));
			return v;
		}

		static vec2 DragFloat2(std::string_view label, vec2 v, float v_speed = 1) {
			ImGui::DragFloat2(label.data(), glm::value_ptr(v), v_speed, 0.0f, 0.0f, __autoFormat(v_speed));
			return v;
		}

		static vec3 DragFloat3(std::string_view label, vec3 v, float v_speed = 1) {
			ImGui::DragFloat3(label.data(), glm::value_ptr(v), v_speed, 0.0f, 0.0f, __autoFormat(v_speed));
			return v;
		}

		static bool CheckBox(std::string_view label, bool v) {
			ImGui::Checkbox(label.data(), &v);
			return v;
		}

		static std::string InputText(std::string_view label, std::string s) {
			ImGui::InputText(label.data(), &s);
			return std::move(s);
		}

		template <typename E>
		static E ComboEnum(std::string_view label, E e) {
			E result {e};
			if (ImGui::BeginCombo(label.data(), magic_enum::enum_name(e).data())) {
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
		static void ComboEnum(std::string_view label, E e, auto&& onSelected) {
			if (ImGui::BeginCombo(label.data(), magic_enum::enum_name(e).data())) {
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

namespace RoamerEditor = qy::cg::editor;