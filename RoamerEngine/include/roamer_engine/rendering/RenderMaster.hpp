#pragma once
#include "../Object.hpp"
#include "../display/Light.hpp"

namespace qy::cg::rendering {

	class RenderMaster {

		struct Light {
			int type;				// 0
			float range;			// 4
			float cutOff;			// 8
			float outerCutOff;		// 12
			glm::vec4 ambient;		// 16
			glm::vec4 diffuse;		// 32
			glm::vec4 specular;		// 48
			glm::vec3 position;		// 64
			float __0;				// 76
			glm::vec3 direction;	// 80
			float __1;				// 92
		};

		struct Lights {
			glm::vec3 viewPos;
			int numLights;
			glm::vec4 globalAmbient;
			Light lights[256];
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
		void lighting(const std::vector<qy::cg::Light*>& lightList, glm::vec3 viewPos, glm::vec4 globalAmbient);

	private:
		GLuint uboLights;
		Lights lights;
	};

}