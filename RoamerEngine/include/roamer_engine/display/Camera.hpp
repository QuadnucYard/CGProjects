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

		void render();

	private:
		DECL_PIMPL;
	};

}