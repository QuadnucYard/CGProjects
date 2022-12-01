#include "roamer_engine/rendering/RenderMaster.hpp"
#include "roamer_engine/display/Transform.hpp"
#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Light.hpp"
#include "roamer_engine/display/Renderer.hpp"
#include "roamer_engine/display/Scene.hpp"
#include "roamer_engine/display/Skybox.hpp"
#include "roamer_engine/display/Shader.hpp"
#include "roamer_engine/display/Material.hpp"
#include "roamer_engine/Screen.hpp"

namespace qy::cg::rendering {

	RenderMaster* RenderMaster::instance() {
		static RenderMaster _instance;
		return &_instance;
	}

	RenderMaster::RenderMaster(): uboCamera(0), uboLights(2) {
		maxTextures = []() {
			int iUnits;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &iUnits);
			return iUnits;
		}();
	}

	void RenderMaster::setCamera(const Camera* camera) {
		uboCamera->viewPos = camera->transform()->position();
		uboCamera->view = camera->viewMatrix();
		uboCamera->proj = camera->projMatrix();
		uboCamera.upload();
	}

	void RenderMaster::lighting(const std::vector<qy::cg::Light*>& lightList) {
		uboLights->numLights = static_cast<int>(lightList.size());
		uboLights->globalAmbient = Scene::current()->getAmbientColor();
		uboLights->farPlane = ShadowMapping::FAR_PLANE;
		int s1 = 0, s2 = 0; // direct和point的数量
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

			if (light->getShadows() == LightShadow::None) {
				uboLight.shadows = 0;
			} else {
				bool isPoint = light->getType() == LightType::Point;
				if (isPoint) {
					if (s2 >= NUM_POINT_SHADOWMAP) continue;
					s2++;
				} else {
					if (s1 >= NUM_DIRECT_SHADOWMAP) continue;
					uboLights->lightSpaceMatrices[s1] = directShadowMaps[s1].lightSpaceMatrix;
					s1++;
				}

				switch (light->getShadows()) {
					case LightShadow::Hard:
						uboLight.shadows = isPoint ? s2 : s1;
						uboLight.shadowStrength = light->getShadowStrength();
						break;
					case LightShadow::Soft:
						uboLight.shadows = -(isPoint ? s2 : s1);
						uboLight.shadowStrength = light->getShadowStrength();
						break;
				}
			}
		}
		uboLights->numDirectShadows = s1;
		uboLights->numPointShadows = s2;
		uboLights.upload();
	}

	void RenderMaster::pass(const Camera* camera) {

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
				if (!child->obj()->activeSelf()) continue;
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

		int numDirectShadows = 0, numPointShadows = 0;
		for (auto&& light : lights) {
			if (light->getShadows() == LightShadow::None) continue;
			Shader* depthShader {nullptr};
			LightType lightType = light->getType();
			switch (lightType) {
				case LightType::Directional:
				case LightType::Spot:
					if (numDirectShadows == directShadowMaps.size()) break;
					depthShader = directShadowMaps[numDirectShadows].shadowing(lightType, light->transform()->position());
					++numDirectShadows;
					break;
				case LightType::Point:
					if (numPointShadows == pointShadowMaps.size()) break;
					depthShader = pointShadowMaps[numPointShadows].shadowing(lightType, light->transform()->position());
					++numPointShadows;
					break;
			}
			if (!depthShader) continue;
			// Not the active shader is depth shader.
			for (auto&& r : renderList) {
				depthShader->setMat4("model", r.model);
				r.renderer->__render();
			}
		}
		// All depthMaps should have values.
		// 
		for (int i = 0; i < directShadowMaps.size(); i++) {
			glBindTextureUnit(maxTextures - i - 1, directShadowMaps[i].depthTexture);
			//glActiveTexture(GL_TEXTURE0 + maxTextures - i - 1);
			//glBindTexture(GL_TEXTURE_2D, directShadowMaps[i].depthTexture); // 可以赋值0！
		}
		for (int i = 0; i < pointShadowMaps.size(); i++) {
			glBindTextureUnit(maxTextures - i - 1 - (int)directShadowMaps.size(), pointShadowMaps[i].depthTexture);
			//glActiveTexture(GL_TEXTURE0 + maxTextures - i - 1 - (int)directShadowMaps.size());
			//glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowMaps[i].depthTexture);
		}

		// render scene as normal 

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Screen::width(), Screen::height());
		camera->clearBuffer();

		// Lighting
		lighting(lights);

		// Render
		for (auto&& r : renderList) {
			for (auto&& mat : r.renderer->__getMaterials()) {
				auto&& shader = mat->getShader();
				shader.use();
				shader.setMat4("model", r.model);
				mat->__applyProperties();
				// All depthMaps should have values
				// Maybe this only need do once
				for (int i = 0; i < directShadowMaps.size(); i++)
					shader.setInt(std::format("depthMaps[{}]", i), maxTextures - 1 - i);
				for (int i = 0; i < pointShadowMaps.size(); i++)
					shader.setInt(std::format("depthCubemaps[{}]", i), maxTextures - 1 - i - (int)directShadowMaps.size());
			}
			r.renderer->__render();
		}

		// Render SkyBox
		if (camera->getClearFlags() == CameraClearFlags::Skybox) {
			camera->getComponent<SkyBox>()->__render(uboCamera->view, uboCamera->proj);
		}
	}
}