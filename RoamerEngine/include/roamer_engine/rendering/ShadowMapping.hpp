#pragma once
#include "../Object.hpp"
#include "../display/Shader.hpp"

namespace qy::cg {
	class Light;
};

namespace qy::cg::rendering {

	class ShadowMapping {

	public:
		ShadowMapping();

		virtual void activate() = 0;
		virtual Shader* shadowing(const Light* light);

	protected:
		void initFrameBuffer();

	public:
		static constexpr unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
		static constexpr float SHADOW_ASPECT = static_cast<float>(SHADOW_WIDTH) / SHADOW_HEIGHT;
		inline static constexpr float NEAR_PLANE = 0.1f, FAR_PLANE = 25.0f;

		unsigned int depthMapFBO {0};
		unsigned int depthTexture {0};
	};

	class DirectShadowMapping: public ShadowMapping {

	public:
		DirectShadowMapping();

		void activate() override;
		Shader* shadowing(const Light* light) override;

	public:
		glm::mat4 lightSpaceMatrix {};

	private:
		inline static Shader depthShader;
	};

	class PointShadowMapping: public ShadowMapping {

	public:
		PointShadowMapping();

		void activate() override;
		Shader* shadowing(const Light* light) override;

	private:
		inline static Shader depthShader;
	};
}