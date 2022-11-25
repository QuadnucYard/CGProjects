#include "roamer_engine/rendering/RenderMaster.hpp"
#include "roamer_engine/display/Transform.hpp"
#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Light.hpp"
#include "roamer_engine/display/Scene.hpp"
#include "roamer_engine/display/Skybox.hpp"
#include "roamer_engine/display/Shader.hpp"
#include "roamer_engine/display/Material.hpp"
#include "roamer_engine/Screen.hpp"
#include <format>

namespace qy::cg::rendering {

	RenderMaster::RenderMaster(): uboCamera(0), uboLights(2) {
		maxTextures = []() {
			int iUnits;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &iUnits);
			return iUnits;
		}();
	}

	void RenderMaster::setCamera(Camera* camera) {
		uboCamera->viewPos = camera->transform()->position();
		uboCamera->view = camera->viewMatrix();
		uboCamera->proj = camera->projMatrix();
		uboCamera.upload();
	}

	void RenderMaster::shadowing(int index, Light* light) {
		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		const glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 
			(float)ShadowMapping::SHADOW_WIDTH / ShadowMapping::SHADOW_HEIGHT, 
			ShadowMapping::NEAR_PLANE, ShadowMapping::FAR_PLANE);

		const ShadowMapping& shadowMap = shadowMaps[index];

		glViewport(0, 0, ShadowMapping::SHADOW_WIDTH, ShadowMapping::SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		auto lightPos = light->transform()->position();
		glm::mat4 shadowTransforms[] {
			shadowProj * glm::lookAt(lightPos, lightPos + vec3(1.0f, 0.0f, 0.0f), {0.0f, -1.0f, 0.0f}),
			shadowProj * glm::lookAt(lightPos, lightPos + vec3(-1.0f, 0.0f, 0.0f), {0.0f, -1.0f, 0.0f}),
			shadowProj * glm::lookAt(lightPos, lightPos + vec3(0.0f, 1.0f, 0.0f), {0.0f, 0.0f, 1.0f}),
			shadowProj * glm::lookAt(lightPos, lightPos + vec3(0.0f, -1.0f, 0.0f), {0.0f, 0.0f, -1.0f}),
			shadowProj * glm::lookAt(lightPos, lightPos + vec3(0.0f, 0.0f, 1.0f), {0.0f, -1.0f, 0.0f}),
			shadowProj * glm::lookAt(lightPos, lightPos + vec3(0.0f, 0.0f, -1.0f), {0.0f, -1.0f, 0.0f}),
		};
		// 1. render scene to depth cubemap
		// --------------------------------
		shadowMap.simpleDepthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			shadowMap.simpleDepthShader.setMat4(std::format("shadowMatrices[{}]", i), shadowTransforms[i]);
		shadowMap.simpleDepthShader.setVec3("lightPos", lightPos);
	}

	void RenderMaster::lighting(const std::vector<qy::cg::Light*>& lightList, glm::vec4 globalAmbient) {
		uboLights->numLights = static_cast<int>(lightList.size());
		uboLights->globalAmbient = globalAmbient;
		uboLights->farPlane = ShadowMapping::FAR_PLANE;
		int s = 0;
		for (size_t i = 0; i < lightList.size(); i++) {
			auto&& light = lightList[i];
			auto&& uboLight = uboLights->lights[i];
			uboLight.type = static_cast<int>(light->getType());
			uboLight.ambient = light->getAmbient() * light->getIntensity();
			uboLight.diffuse = light->getDiffuse() * light->getIntensity();
			uboLight.specular = light->getSpecular() * light->getIntensity();
			uboLight.position = light->transform()->position();
			uboLight.direction = light->transform()->rotation() * glm::vec3(0, 0, -1.0f);
			uboLight.range = light->getRange();
			uboLight.outerCutOff = glm::cos(glm::radians(light->getSpotAngle()));
			uboLight.cutOff = glm::cos(glm::radians(light->getInnerSpotAngle()));
			switch (light->getShadows()) {
				case LightShadow::None:
					uboLight.shadows = 0;
					break;
				case LightShadow::Hard:
					uboLight.shadows = ++s;
					uboLight.shadowStrength = light->getShadowStrength();
					break;
				case LightShadow::Soft:
					uboLight.shadows = -(++s);
					uboLight.shadowStrength = light->getShadowStrength();
					break;
			}
		}
		uboLights.upload();
	}

	void RenderMaster::pass(Camera* camera) {

		// Get renderers and lights 

		struct RenderItem {
			int renderOrder;
			int layerOrder;
			Renderer* renderer;
			glm::mat4 model;
		};
		std::vector<RenderItem> renderList;
		std::vector<Light*> lights;

		const auto dfs = [&](this auto&& self, const TransformPtr& parent, const glm::mat4& model) -> void {
			int i = 0;
			for (auto&& child : parent) {
				auto model2 = model * child->modelMatrix();
				for (auto&& r : child->getComponents<Renderer>()) {
					if (r->enabled())
						renderList.emplace_back(0, i, r.get(), model2);
				}
				for (auto&& light : child->getComponents<Light>()) {
					if (light->enabled())
						lights.push_back(light.get());
				}
				self(child, model2);
				i++;
			}
		};

		dfs(Scene::current()->root(), Scene::current()->root()->modelMatrix());

		// Sort render items
		std::ranges::sort(renderList, [](const RenderItem& o1, const RenderItem& o2) {
			return std::tie(o1.renderOrder, o2.layerOrder) < std::tie(o2.renderOrder, o2.layerOrder);
		});

		setCamera(camera);

		// Shadow rendering

		int numShadows = 0;
		for (auto&& light : lights) {
			if (light->getShadows() == LightShadow::None) continue;
			shadowing(numShadows, light);
			// Not the active shader is depth shader.
			for (auto&& r : renderList) {
				shadowMaps[numShadows].simpleDepthShader.setMat4("model", r.model);
				r.renderer->__render();
			}
			if (++numShadows == shadowMaps.size()) break;
		}
		// All depthMaps should have values.
		for (int i = 0; i < shadowMaps.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + maxTextures - i - 1);
			glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMaps[i].depthCubemap);
		}

		// render scene as normal 

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Screen::width(), Screen::height());
		camera->clearBuffer();

		// Lighting
		lighting(lights, Scene::current()->getAmbientColor());

		// Render
		for (auto&& r : renderList) {
			for (auto&& mat : r.renderer->__getMaterials()) {
				auto&& shader = mat->getShader();
				shader.use();
				shader.setMat4("model", r.model);
				auto a1 = shader.locAttrib("_MainTex");
				auto a2 = shader.locAttrib("_SpecTex");
				auto a3 = shader.locAttrib("depthMaps[0]");
				auto a4 = shader.locAttrib("depthMaps[1]");
				auto a5 = shader.locAttrib("model");
				auto a6 = shader.locAttrib("material.ambient");
				mat->__applyProperties();
				// All depthMaps should have values
				for (int i = 0; i < shadowMaps.size(); i++)
					shader.setInt(std::format("depthMaps[{}]", i), maxTextures - 1 - i);
			}
			r.renderer->__render();
		}

		// Render SkyBox
		if (camera->getClearFlags() == CameraClearFlags::Skybox) {
			camera->getComponent<SkyBox>()->__render(uboCamera->view, uboCamera->proj);
		}
	}
}