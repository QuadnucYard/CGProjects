#pragma once
#include "EditorGUI.hpp"
#include <memory>

namespace qy::cg {
	class Component;
}

namespace qy::cg::editor {

	class EditorBase : public EditorGUI {
	public:
		virtual void start() {}
		virtual void onInspectorGUI() {}
	};

	template <class T>
	class Editor : public EditorBase {

	public:
		//Editor(const std::shared_ptr<Component>& target) : target(std::dynamic_pointer_cast<T>(target)) {}
		void setTarget(const std::shared_ptr<Component>& target) {
			this->target = std::dynamic_pointer_cast<T>(target);
		}

	protected:
		std::weak_ptr<T> target;
	};

}