#pragma once
#include "../Object.hpp"

namespace qy::cg {

	class Transform;
	class Camera;

	class Scene : public Object {
	private:
		inline static ptr<Scene> _currentScene;

	public:
		DECL_OBJECT(Scene);

		static ptr<Scene> create();

		static ptr<Scene> current();

		ptr<Transform> root();

		ptr<Camera> createCamera();

		void dispatch_update();

		void dispatch_render();

		color_t getAmbientColor() const;
		void setAmbientColor(color_t value);

	private:
		DECL_PIMPL;
	};
}