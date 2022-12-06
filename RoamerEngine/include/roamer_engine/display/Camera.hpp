#pragma once
#include "DisplayObject.hpp"

namespace qy::cg {

	enum class CameraClearFlags {
		Skybox,
		SolidColor,
		Depth,
		Nothing,
	};


	class Camera: public Component {

	public:
		inline static ptr<Camera> s_main;

		DECL_OBJECT(Camera);

		static ptr<Camera> main() {
			return s_main;
		}

		void start() override;

		float getAspect() const;
		void setAspect(float value);
		float getDepth() const;
		void setDepth(float value);
		float getFieldOfView() const;
		void setFieldOfView(float value);
		float getNearClipPlane() const;
		void setNearClipPlane(float value);
		float getFarClipPlane() const;
		void setFarClipPlane(float value);
		bool isOrthographic() const;
		void setOrthographic(bool value);
		float getOrthographicSize() const;
		void setOrthographicSize(float value);
		CameraClearFlags getClearFlags() const;
		void setClearFlags(CameraClearFlags value);
		glm::vec4 getBackgroundColor() const;
		void setBackgroundColor(glm::vec4 value);

		mat4 viewMatrix() const;
		mat4 projMatrix() const;

		void resetAspect();

		void clearBuffer() const;

		void render();

	private:
		DECL_PIMPL;
	};

}