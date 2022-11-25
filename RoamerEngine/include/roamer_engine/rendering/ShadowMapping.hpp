#pragma once
#include "../Object.hpp"
#include "../display/Shader.hpp"

namespace qy::cg {

	class ShadowMapping {

	public:
		ShadowMapping();

	public:
		static const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		inline static const float NEAR_PLANE = 0.1f, FAR_PLANE = 25.0f;
		inline static Shader simpleDepthShader;

		unsigned int depthMapFBO;
		unsigned int depthCubemap;
	};
}