#pragma once
#include "../Object.hpp"
#include "../display/Shader.hpp"

namespace qy::cg {

	class ShadowMapping {

	private:
		inline static ShadowMapping* _instance {nullptr};
	public:
		static ShadowMapping* instance() { return _instance ? _instance : (_instance = new ShadowMapping()); }
	
		ShadowMapping();

	public:
		static const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		unsigned int depthMapFBO;
		unsigned int depthCubemap;
		Shader simpleDepthShader;
	};
}