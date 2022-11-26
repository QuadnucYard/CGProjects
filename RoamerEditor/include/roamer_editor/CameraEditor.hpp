#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Camera.hpp>

namespace qy::cg::editor {

	class CameraEditor: public Editor<Camera> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override {
			auto&& camera = target.lock();
			
			camera->setAspect(DragFloat("Aspect", camera->getAspect(), 0.01f));
			camera->setFieldOfView(DragFloat("FieldOfView", camera->getFieldOfView(), 1.0f));
		}
	};

}