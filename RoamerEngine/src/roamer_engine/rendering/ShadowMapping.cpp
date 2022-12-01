#include "roamer_engine/rendering/ShadowMapping.hpp"
#include "roamer_engine/display/Shaders.hpp"
#include <format>

namespace qy::cg {

	ShadowMapping::ShadowMapping() {
		// Need to activate
	}

	Shader* ShadowMapping::shadowing(LightType type, vec3 lightPos) {
		activate();
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		return nullptr;
	}

	void ShadowMapping::initFrameBuffer() {
		// attach depth texture as FBO's depth buffer
		glGenFramebuffers(1, &depthMapFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	DirectShadowMapping::DirectShadowMapping() {
		if (!depthShader) {
			depthShader = Shader::fromSourceFile(
				Shaders::ResPath / "direct-shadow-depth.vert",
				Shaders::ResPath / "direct-shadow-depth.frag"
			);
		}
	}

	void DirectShadowMapping::activate() {
		if (depthTexture) return;
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float borderColor[] = {1.0, 1.0, 1.0, 1.0};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		initFrameBuffer();
	}

	Shader* DirectShadowMapping::shadowing(LightType type, vec3 lightPos) {
		static const glm::mat4 DIR_PROJ {glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, NEAR_PLANE, FAR_PLANE)};
		static const glm::mat4 SPOT_PROJ {glm::perspective(glm::radians(45.0f), SHADOW_ASPECT, NEAR_PLANE, FAR_PLANE)};
		
		ShadowMapping::shadowing(type, lightPos);
		// TODO 这里没考虑光源方向
		const glm::mat4& lightProjection = type == LightType::Directional ? DIR_PROJ : SPOT_PROJ;
		glm::mat4 lightView = glm::lookAt(lightPos, vec3(0.0f), {0.0, 1.0, 0.0});
		lightSpaceMatrix = lightProjection * lightView;
		depthShader.use();
		depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		return &depthShader;
	}

	PointShadowMapping::PointShadowMapping() {
		if (!depthShader) {
			depthShader = Shader::fromSourceFile(
				Shaders::ResPath / "point-shadow-depth.vert",
				Shaders::ResPath / "point-shadow-depth.frag",
				Shaders::ResPath / "point-shadow-depth.geom"
			);
			depthShader.use();
			depthShader.setFloat("farPlane", FAR_PLANE);
		}
	}

	void PointShadowMapping::activate() {
		if (depthTexture) return;
		// create depth cubemap texture
		glGenTextures(1, &depthTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);
		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		initFrameBuffer();
	}

	Shader* PointShadowMapping::shadowing(LightType type, vec3 lightPos) {
		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		static const glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), SHADOW_ASPECT, NEAR_PLANE, FAR_PLANE);

		ShadowMapping::shadowing(type, lightPos);

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
		depthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			depthShader.setMat4(std::format("shadowMatrices[{}]", i), shadowTransforms[i]);
		depthShader.setVec3("lightPos", lightPos);
		return &depthShader;
	}

}
