#pragma once
#include "DisplayObject.hpp"

namespace qy::cg {

	class Camera: public Component {

	public:
		inline static std::shared_ptr<Camera> s_main;

		DECL_OBJECT(Camera);

		static std::shared_ptr<Camera> main() {
			return s_main;
		}

		void start() override {
			s_main = std::dynamic_pointer_cast<Camera>(shared_from_this());
		}

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

		void render();

	private:
		DECL_PIMPL;
	};

}