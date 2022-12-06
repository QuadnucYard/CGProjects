#pragma once
#include "EditorGUI.hpp"
#include <memory>
#include <unordered_map>

namespace qy::cg {
	class Component;
}

namespace qy::cg::editor {

	class Editor : public EditorGUI {

	public:
		//Editor(const std::shared_ptr<Component>& target) : target(std::dynamic_pointer_cast<T>(target)) {}
		void setTarget(const std::shared_ptr<Component>& target) {
			this->target = target;
		}

		template <class T>
		ptr<T> lockTarget() const {
			return std::dynamic_pointer_cast<T>(target.lock());
		}

		virtual void start() {}
		virtual void onInspectorGUI() {}

	public:
		std::weak_ptr<Component> target;
	};


	struct EditorFactory {
		inline static std::unordered_map<std::string, std::string> target_table;

		template <class T>
		struct RegistryHelper {
			RegistryHelper(const std::string& editorName, const std::string& targetName) {
				target_table.emplace(targetName, editorName);
				ObjectFactory::type_creator_map.emplace(editorName, [] { return new T(); });
			}
		};

		static std::unique_ptr<Editor> create(const std::string& targetName) {
			if (!target_table.contains(targetName)) return nullptr;
			return ObjectFactory::create<Editor>(target_table.at(targetName));
		}

	};


#define RegisterEditor(Editor, Target) \
	private: inline static EditorFactory::RegistryHelper<Editor> __helper##Editor##Target {#Editor, #Target};


}