#pragma once
#include "../Object.hpp"
#include "UniformBufferObject.hpp"
#include "ShadowMapping.hpp"
#include <array>

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
			float farPlane;
			int __0[2];
			glm::vec4 globalAmbient;
			LightUBO lights[256];
		};

	private:
		inline static RenderMaster* _instance {nullptr};
	public:
		static RenderMaster* instance() {
			if (!_instance) _instance = new RenderMaster();
			return _instance;
		}

		RenderMaster();

	public:
		void setCamera(Camera* camera);

		void shadowing(int index, Light* light);

		void lighting(const std::vector<Light*>& lightList, glm::vec4 globalAmbient);

		void pass(Camera* camera);

	private:
		UniformBufferObject<CameraUBO> uboCamera;
		UniformBufferObject<LightsUBO> uboLights;
		std::array<ShadowMapping, 16> shadowMaps;
		inline static int maxTextures;
	};

}