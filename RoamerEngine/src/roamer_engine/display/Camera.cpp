#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Scene.hpp"
#include "roamer_engine/display/MeshFilter.hpp"
#include "roamer_engine/display/Shader.hpp"
#include "roamer_engine/display/SkyBox.hpp"
#include "roamer_engine/display/Light.hpp"
#include "roamer_engine/rendering/RenderMaster.hpp"

namespace qy::cg {

	struct Camera::Impl {
		float aspect {16.0f / 9.0f}; // The aspect ratio (width divided by height).
		float depth {0}; // Camera's depth in the camera rendering order.
		float fieldOfView {90.0f}; // The vertical field of view of the Camera, in degrees.
		float nearClipPlane {0.1f}; // The distance of the near clipping plane from the the Camera, in world units.
		float farClipPlane {1000.0f}; // The distance of the far clipping plane from the Camera, in world units.

		bool orthographic {false}; // Is the camera orthographic (true) or perspective (false)?
		float orthographicSize {10.0f}; // Camera's half-size when in orthographic mode.

		CameraClearFlags clearFlags {CameraClearFlags::SolidColor};
		glm::vec4 backgroundColor {};

	};

	DEFINE_OBJECT(Camera);

	float Camera::getAspect() const { return pImpl->aspect; }
	void Camera::setAspect(float value) { pImpl->aspect = value; }
	float Camera::getDepth() const { return pImpl->depth; }
	void Camera::setDepth(float value) { pImpl->depth = value; }
	float Camera::getFieldOfView() const { return pImpl->fieldOfView; }
	void Camera::setFieldOfView(float value) { pImpl->fieldOfView = value; }
	float Camera::getNearClipPlane() const { return pImpl->nearClipPlane; }
	void Camera::setNearClipPlane(float value) { pImpl->nearClipPlane = value; }
	float Camera::getFarClipPlane() const { return pImpl->farClipPlane; }
	void Camera::setFarClipPlane(float value) { pImpl->farClipPlane = value; }
	bool Camera::isOrthographic() const { return pImpl->orthographic; }
	void Camera::setOrthographic(bool value) { pImpl->orthographic = value; }
	float Camera::getOrthographicSize() const { return pImpl->orthographicSize; }
	void Camera::setOrthographicSize(float value) { pImpl->orthographicSize = value; }
	CameraClearFlags Camera::getClearFlags() const { return pImpl->clearFlags; }
	void Camera::setClearFlags(CameraClearFlags value) { pImpl->clearFlags = value; }
	glm::vec4 Camera::getBackgroundColor() const { return pImpl->backgroundColor; }
	void Camera::setBackgroundColor(glm::vec4 value) { pImpl->backgroundColor = value; }

	void Camera::render() {

		struct RenderItem {
			int renderOrder;
			int layerOrder;
			Renderer* renderer;
			glm::mat4 model;
		};
		std::vector<RenderItem> renderList;
		std::vector<Light*> lights;

		const auto& dfs = [&](const TransformPtr& parent, const glm::mat4& model) {
			const auto& s = [&](auto&& self, const TransformPtr& parent, const glm::mat4& model) -> void {
				int i = 0;
				for (auto&& child : parent) {
					auto model2 = model * child->modelMatrix();
					for (auto&& r : child->getComponents<Renderer>()) {
						renderList.emplace_back(0, i, r.get(), model2);
					}
					for (auto&& light : child->getComponents<Light>()) {
						lights.push_back(light.get());
					}
					self(self, child, model2);
					i++;
				}
			};
			s(s, parent, model);
		};

		dfs(Scene::current()->root(), Scene::current()->root()->modelMatrix());

		std::ranges::sort(renderList, [](const RenderItem& o1, const RenderItem& o2) {
			return std::tie(o1.renderOrder, o2.layerOrder) < std::tie(o2.renderOrder, o2.layerOrder);
		});

		if (pImpl->clearFlags == CameraClearFlags::SolidColor || pImpl->clearFlags == CameraClearFlags::Skybox) {
			auto&& c = pImpl->backgroundColor;
			glClearColor(c.r, c.g, c.b, c.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		} else if (pImpl->clearFlags ==  CameraClearFlags::Depth) {
			auto&& c = pImpl->backgroundColor;
			glClearColor(c.r, c.g, c.b, c.a);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		auto view = glm::lookAt(transform()->position(), transform()->position() + transform()->rotation() * glm::vec3 {0, 0, -1}, {0, 1, 0});
		glm::mat4 proj;
		if (isOrthographic()) {
			float h = getOrthographicSize(), w = getOrthographicSize() * getAspect();
			proj = glm::ortho(-w, w, -h, h, pImpl->nearClipPlane, pImpl->farClipPlane);
		} else {
			proj = glm::perspective(glm::radians(pImpl->fieldOfView), pImpl->aspect, pImpl->nearClipPlane, pImpl->farClipPlane);
		}

		// Lighting
		rendering::RenderMaster::instance()->lighting(lights, transform()->position(), Scene::current()->getAmbientColor());

		// Now only support MeshRenderer
		for (auto&& r : renderList) {
			for (auto&& mat : r.renderer->__getMaterials()) {
				auto&& shader = mat->getShader();
				shader.use();
				shader.setMat4("model", r.model);
				shader.setMat4("view", view);
				shader.setMat4("proj", proj);
				mat->__applyProperties();
			}
			auto mf = r.renderer->getComponent<MeshFilter>();
			mf->mesh()->__render();
		}

		// Render SkyBox
		if (pImpl->clearFlags == CameraClearFlags::Skybox) {
			getComponent<SkyBox>()->__render(view, proj);
		}
	}

}