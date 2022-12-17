#pragma once
#include "../display/Shader.hpp"

namespace qy::cg {

	class HDRBuffer {
	public:
		void setup();

		void activate();

		void render();

	private:
		void renderQuad();

	private:
		Shader hdrShader;
		GLuint hdrFBO {0};
		GLuint rboDepth {0};
		GLuint colorBuffer {0};
	};

}