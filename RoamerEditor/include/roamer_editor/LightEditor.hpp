#pragma once
#include "Editor.hpp"
#include <roamer_engine/display/Light.hpp>
#include <unordered_map>
#include <magic_enum.hpp>
#include <nameof.hpp>

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

			//ComboEnum("LightType", light->getType(), [&](auto t) { light->setType(t); });
			switch (auto type = ComboEnum("Type", light->getType()); light->setType(type), type) {
				case LightType::Spot:
					light->setSpotAngle(DragFloat("SpotAngle", light->getSpotAngle()));
					light->setInnerSpotAngle(DragFloat("InnerSpotAngle", light->getInnerSpotAngle()));
					[[fallthrough]];
				case LightType::Point:
					light->setRange(DragFloat("Range", light->getRange(), 0.1f));
					[[fallthrough]];
				case LightType::Directional:
					break;
			}

			switch (auto shadows = ComboEnum("Shadows", light->getShadows()); light->setShadows(shadows), shadows) {
				case LightShadow::Hard:
				case LightShadow::Soft:
					light->setShadowStrength(DragFloat("ShadowStrength", light->getShadowStrength(), 0.01f));
					break;
			}

		}
	};

}