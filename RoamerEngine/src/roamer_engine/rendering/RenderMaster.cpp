#include "roamer_engine/rendering/RenderMaster.hpp"
#include "roamer_engine/display/Transform.hpp"

namespace qy::cg::rendering {

	RenderMaster::RenderMaster() {
		glGenBuffers(1, &uboLights);

		glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Lights), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0); // 这里可能是0？
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboLights, 0, sizeof(Lights));
	}

	void RenderMaster::lighting(const std::vector<qy::cg::Light*>& lightList, glm::vec3 viewPos, glm::vec4 globalAmbient) {
		lights.numLights = lightList.size();
		lights.globalAmbient = globalAmbient; sizeof(Light); alignof(Light);
		lights.viewPos = viewPos;
		for (size_t i = 0; i < lightList.size(); i++) {
			auto&& light = lightList[i];
			lights.lights[i].type = static_cast<int>(light->getType());
			lights.lights[i].ambient = light->getAmbient() * light->getIntensity();
			lights.lights[i].diffuse = light->getDiffuse() * light->getIntensity();
			lights.lights[i].specular = light->getSpecular() * light->getIntensity();
			lights.lights[i].position = light->transform()->position();
			auto dir = light->transform()->rotation() * glm::vec3(0, 0, 1.0f);
			lights.lights[i].direction = light->transform()->rotation() * glm::vec3(0, 0, -1.0f);
			lights.lights[i].range = light->getRange();
			lights.lights[i].spotAngle = glm::radians(light->getSpotAngle());
			lights.lights[i].innerSpotAngle = glm::radians(light->getInnerSpotAngle());
		}
		glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Lights), &lights);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}