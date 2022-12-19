#include "roamer_engine/display/Camera.hpp"
#include "roamer_engine/display/Transform.hpp"
#include "roamer_engine/rendering/RenderMaster.hpp"
#include "roamer_engine/Screen.hpp"

namespace qy::cg {

	struct Camera::Impl {
		float aspect {16.0f / 9.0f}; // The aspect ratio (width divided by height).
		float depth {0}; // Camera's depth in the camera rendering order.
		float fieldOfView {90.0f}; // The vertical field of view of the Camera, in degrees.
		float nearClipPlane {0.1f}; // The distance of the near clipping plane from the the Camera, in world units.
		float farClipPlane {1000.0f}; // The distance of the far clipping plane from the Camera, in world units.

		bool orthographic {false}; // Is the camera orthographic (true) or perspective (false)?
		float orthographicSize {10.0f}; // Camera's half-size when in orthographic mode.

		bool gammaCorrection {false};

		CameraClearFlags clearFlags {CameraClearFlags::SolidColor};
		glm::vec4 backgroundColor {};

	};

	DEFINE_OBJECT(Camera);

	inline void Camera::start() {
		s_main = std::dynamic_pointer_cast<Camera>(shared_from_this());
		resetAspect();
	}

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
	bool Camera::getGammaCorrection() const { return pImpl->gammaCorrection; }
	void Camera::setGammaCorrection(bool value) { pImpl->gammaCorrection = value; }

	mat4 Camera::viewMatrix() const {
		vec3 pos = transform()->worldPosition();
		quat rot = transform()->worldRotation();
		return glm::lookAt(
			pos,
			pos + rot * vec3_back,
			rot * vec3_up
		);
	}

	mat4 Camera::projMatrix() const {
		if (isOrthographic()) {
			float h = getOrthographicSize(), w = getOrthographicSize() * getAspect();
			return glm::ortho(-w, w, -h, h, pImpl->nearClipPlane, pImpl->farClipPlane);
		} else {
			return glm::perspective(glm::radians(pImpl->fieldOfView), pImpl->aspect, pImpl->nearClipPlane, pImpl->farClipPlane);
		}
	}

	void Camera::resetAspect() {
		setAspect((float)Screen::width() / Screen::height());
	}

	void Camera::clearBuffer() const {
		if (pImpl->clearFlags == CameraClearFlags::SolidColor || pImpl->clearFlags == CameraClearFlags::Skybox) {
			auto&& c = pImpl->backgroundColor;
			glClearColor(c.r, c.g, c.b, c.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		} else if (pImpl->clearFlags == CameraClearFlags::Depth) {
			auto&& c = pImpl->backgroundColor;
			glClearColor(c.r, c.g, c.b, c.a);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	void Camera::render() {
		rendering::RenderMaster::instance()->pass(this);
	}

}