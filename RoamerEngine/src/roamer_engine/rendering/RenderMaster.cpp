#include "roamer_engine/rendering/RenderMaster.hpp"
#include "roamer_engine/display/Transform.hpp"
#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Light.hpp"

namespace qy::cg::rendering {

	RenderMaster::RenderMaster() : uboCamera(0), uboLights(2) {
		//glCompileShader()
	}

	void RenderMaster::setCamera(Camera* camera) {
		uboCamera->viewPos = camera->transform()->position();
		uboCamera->view = camera->viewMatrix();
		uboCamera->proj = camera->projMatrix();
		uboCamera.upload();
	}

	void RenderMaster::prepareShadowing() {
		glViewport(0, 0, ShadowMapping::SHADOW_WIDTH, ShadowMapping::SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.depthMapFBO);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowMap.simpleDepthShader.use();
	}

	void RenderMaster::shadowing(Light* light) {
		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		const float near_plane = 0.1f;
		const float far_plane = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)ShadowMapping::SHADOW_WIDTH / ShadowMapping::SHADOW_HEIGHT, near_plane, far_plane);
		auto lightPos = light->transform()->position();
		std::vector<glm::mat4> shadowTransforms {
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
			shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		};
		// 1. render scene to depth cubemap
		// --------------------------------
		shadowMap.simpleDepthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			shadowMap.simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		shadowMap.simpleDepthShader.setFloat("far_plane", far_plane);
		shadowMap.simpleDepthShader.setVec3("lightPos", lightPos);
		// 它是对每个光源渲染一次！
	}

	void RenderMaster::lighting(const std::vector<qy::cg::Light*>& lightList, glm::vec4 globalAmbient) {
		uboLights->numLights = static_cast<int>(lightList.size());
		uboLights->globalAmbient = globalAmbient;
		uboLights->farPlane = 25.0f;
		for (size_t i = 0; i < lightList.size(); i++) {
			auto&& light = lightList[i];
			uboLights->lights[i].type = static_cast<int>(light->getType());
			uboLights->lights[i].ambient = light->getAmbient() * light->getIntensity();
			uboLights->lights[i].diffuse = light->getDiffuse() * light->getIntensity();
			uboLights->lights[i].specular = light->getSpecular() * light->getIntensity();
			uboLights->lights[i].position = light->transform()->position();
			uboLights->lights[i].direction = light->transform()->rotation() * glm::vec3(0, 0, -1.0f);
			uboLights->lights[i].range = light->getRange();
			uboLights->lights[i].outerCutOff = glm::cos(glm::radians(light->getSpotAngle()));
			uboLights->lights[i].cutOff = glm::cos(glm::radians(light->getInnerSpotAngle()));
		}
		uboLights.upload();

		// Depth map
		glActiveTexture(GL_TEXTURE31);
		glBindTexture(GL_TEXTURE_CUBE_MAP, rendering::RenderMaster::instance()->shadowMap.depthCubemap);
	}
}