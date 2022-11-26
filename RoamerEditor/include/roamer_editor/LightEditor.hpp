#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Light.hpp>

namespace qy::cg::editor {

	class LightEditor: public Editor<Light> {

	public:
		using Editor::Editor;

		void onInspectorGUI() override {
			auto&& light = target.lock();

			light->setIntensity(DragFloat("Intensity", light->getIntensity(), 0.01f));
			light->setAmbient(ColorEdit4("Ambient", light->getAmbient()));
			light->setDiffuse(ColorEdit4("Diffuse", light->getDiffuse()));
			light->setSpecular(ColorEdit4("Specular", light->getSpecular()));
			
		}
	};

}