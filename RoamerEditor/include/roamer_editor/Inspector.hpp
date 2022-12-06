#pragma once
#include "EditorGUI.hpp"

namespace qy::cg {
	class DisplayObject;
}

namespace qy::cg::editor {

	class Inspector : public EditorGUI {

	public:
		DECL_OBJECT(Inspector);

		static Inspector* instance();
		void inspect(const ptr<DisplayObject>& obj);
		ptr<DisplayObject> inspectedObject() const;
		void onGUI();

	private:
		DECL_PIMPL;
	};

}