#pragma once
#include "../Object.hpp"
#include "UniformBufferObject.hpp"
#include "ShadowMapping.hpp"
#include <array>

#ifndef NUM_DIRECT_SHADOWMAP
#define NUM_DIRECT_SHADOWMAP 8 // If you change this value, together with that in shader.
#endif
#ifndef NUM_POINT_SHADOWMAP
#define NUM_POINT_SHADOWMAP 8 // If you change this value, together with that in shader.
#endif

namespace qy::cg {
	class Camera;
	class Light;
}

namespace qy::cg::rendering {

	class RenderMaster {

		struct CameraUBO {
			glm::vec3 viewPos;
			int __0;
			glm::mat4 view;
			glm::mat4 proj;
		};

		struct LightUBO {
			int type;				// 0
			float range;			// 4
			float cutOff;			// 8
			float outerCutOff;		// 12
			glm::vec4 ambient;		// 16
			glm::vec4 diffuse;		// 32
			glm::vec4 specular;		// 48
			glm::vec3 position;		// 64
			int shadows;			// 76
			glm::vec3 direction;	// 80
			float shadowStrength;	// 92
		};

		struct LightsUBO {
			int numLights;
			int numDirectShadows;
			int numPointShadows;
			float farPlane;
			glm::vec4 globalAmbient;
			glm::mat4 lightSpaceMatrices[NUM_DIRECT_SHADOWMAP];
			LightUBO lights[256];
		};

	public:
		static RenderMaster* instance();

		RenderMaster();

	public:
		void setCamera(const Camera* camera);

		void lighting(const std::vector<Light*>& lightList);

		void pass(const Camera* camera);

	private:
		UniformBufferObject<CameraUBO> uboCamera;
		UniformBufferObject<LightsUBO> uboLights;
		std::array<DirectShadowMapping, NUM_DIRECT_SHADOWMAP> directShadowMaps;
		std::array<PointShadowMapping, NUM_POINT_SHADOWMAP> pointShadowMaps;
		inline static int maxTextures;
	};

}