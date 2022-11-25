#include "roamer_engine/display/Light.hpp"

namespace qy::cg {

	struct Light::Impl {
		LightType type {LightType::Directional};
		color_t ambient {0.0f, 0.0f, 0.0f, 0.0f};
		color_t diffuse {1.0f, 1.0f, 1.0f, 1.0f};
		color_t specular {1.0f, 1.0f, 1.0f, 1.0f};
		float intensity {1};
		float range {10};
		float spotAngle {10};
		float innerSpotAngle {90};
		LightShadow shadows {LightShadow::None};
		float shadowStrength {1.0f};
	};

	DEFINE_OBJECT(Light);

	LightType Light::getType() const { return pImpl->type; }
	void Light::setType(LightType value) { pImpl->type = value; }

	color_t Light::getAmbient() const { return pImpl->ambient; }
	void Light::setAmbient(color_t value) { pImpl->ambient = value; }
	color_t Light::getDiffuse() const { return pImpl->diffuse; }
	void Light::setDiffuse(color_t value) { pImpl->diffuse = value; }
	color_t Light::getSpecular() const { return pImpl->specular; }
	void Light::setSpecular(color_t value) { pImpl->specular = value; }

	float Light::getIntensity() const { return pImpl->intensity; }
	void Light::setIntensity(float value) { pImpl->intensity = value; }
	float Light::getRange() const { return pImpl->range; }
	void Light::setRange(float value) { pImpl->range = value; }
	float Light::getSpotAngle() const { return pImpl->spotAngle; }
	void Light::setSpotAngle(float value) { pImpl->spotAngle = value; }
	float Light::getInnerSpotAngle() const { return pImpl->innerSpotAngle; }
	void Light::setInnerSpotAngle(float value) { pImpl->innerSpotAngle = value; }

	LightShadow Light::getShadows() const { return pImpl->shadows; }
	void Light::setShadows(LightShadow value) { pImpl->shadows = value; }
	float Light::getShadowStrength() const { return pImpl->shadowStrength; }
	void Light::setShadowStrength(float value) { pImpl->shadowStrength = value; }
}

/*
暂定这个公式
a = 1.0
b = 4.6/(r+1)
c = 80/((r+1)^2)

A = a + bd + cd^2
  = a + 4.6(d/r)+80(d/r)^2
*/