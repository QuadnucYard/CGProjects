#pragma once
#include "Component.hpp"

namespace qy::cg {

	enum class LightType {
		Directional, Point, Spot,
	};

	enum class LightShadow {
		None, Hard, Soft,
	};

	class Light: public Component {

	public:
		DECL_OBJECT(Light);

		LightType getType() const;
		void setType(LightType value);

		color_t getAmbient() const;
		void setAmbient(color_t value);
		color_t getDiffuse() const;
		void setDiffuse(color_t value);
		color_t getSpecular() const;
		void setSpecular(color_t value);

		float getIntensity() const;
		void setIntensity(float value);
		float getRange() const;
		void setRange(float value);
		float getSpotAngle() const;
		void setSpotAngle(float value);
		float getInnerSpotAngle() const;
		void setInnerSpotAngle(float value);

		LightShadow getShadows() const;
		void setShadows(LightShadow value);
		float getShadowStrength() const;
		void setShadowStrength(float value);

	private:
		DECL_PIMPL;

	};
}