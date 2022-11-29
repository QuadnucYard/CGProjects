#include "roamer_editor/CameraEditor.hpp"

namespace qy::cg::editor {

	void CameraEditor::onInspectorGUI() {
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
			case CameraClearFlags::Skybox:
				break;
			case CameraClearFlags::SolidColor:
				camera->setBackgroundColor(ColorEdit4("BackgroundColor", camera->getBackgroundColor()));
				break;
			case CameraClearFlags::Depth:
				break;
			case CameraClearFlags::Nothing:
				break;
		}
	}

}