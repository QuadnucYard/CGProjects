#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Camera.hpp>

namespace qy::cg::editor {

	class CameraEditor: public Editor<Camera> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override {
			auto&& camera = target.lock();
			
			if (auto ortho = CheckBox("Orthographic", camera->isOrthographic()); camera->setOrthographic(ortho), ortho) {
				camera->setOrthographicSize(DragFloat("OrthographicSize", camera->getOrthographicSize(), 0.1f));
			} else {
				camera->setFieldOfView(DragFloat("FieldOfView", camera->getFieldOfView(), 0.1f));
			}
			camera->setAspect(DragFloat("Aspect", camera->getAspect(), 0.01f));
			camera->setNearClipPlane(DragFloat("NearClipPlane", camera->getNearClipPlane(), 0.1f));
			camera->setFarClipPlane(DragFloat("FarClipPlane", camera->getFarClipPlane(), 0.1f));
			
			switch (auto clearFlags = ComboEnum("ClearFlags", camera->getClearFlags()); camera->setClearFlags(clearFlags), clearFlags) {
				case qy::cg::CameraClearFlags::Skybox:
					break;
				case qy::cg::CameraClearFlags::SolidColor:
					camera->setBackgroundColor(ColorEdit4("BackgroundColor", camera->getBackgroundColor()));
					break;
				case qy::cg::CameraClearFlags::Depth:
					break;
				case qy::cg::CameraClearFlags::Nothing:
					break;
			}
		}
	};

}