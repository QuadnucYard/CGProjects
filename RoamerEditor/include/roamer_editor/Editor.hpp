#pragma once
#include "EditorGUI.hpp"
#include <memory>

namespace qy::cg {
	class Component;
}

namespace qy::cg::editor {

	template <class T>
	class Editor : public EditorGUI {

	public:
		Editor(const std::shared_ptr<Component>& target): target(std::dynamic_pointer_cast<T>(target)) {}
		virtual void onInspectorGUI() {}

	protected:
		std::weak_ptr<T> target;
	};

}