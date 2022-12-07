#include "roamer_engine/display/MoveController.hpp"
#include <roamer_engine/input.hpp>
#include <roamer_engine/Time.hpp>
#include <roamer_engine/display/Camera.hpp>
#include <roamer_engine/display/Transform.hpp>
#include <roamer_engine/Application.hpp>

namespace qy::cg {

	struct MoveController::Impl {
		glm::vec3 front_init {0.0, 0.0, -1.0};
		glm::quat rot_init {};
		glm::vec3 front;
		glm::vec3 up {0, 1, 0};
		glm::vec3 mousePosLast = {0.0f, 0.0f, 0.0f};
		float sensitivity = 0.1f;
		float pitch;
		float yaw;
		bool firstMouse = true;
		MoveType moveType = MoveType::Flat;
		DirectController directController = DirectController::Mouse;
		float speed = 3;
		std::map<MoveDirection, KeyCode> move2keyMap;
		bool hideMouse = true;
		bool mouseLeftPressed = false;
		bool mouseMidPressed = false;

		Impl() {
			front = front_init;
			pitch = glm::degrees(asin(glm::normalize(front_init).y));
			yaw = glm::degrees(asin(glm::normalize(front_init).z / cos(pitch)));
			move2keyMap.insert_or_assign(MoveDirection::Front, KeyCode::W);
			move2keyMap.insert_or_assign(MoveDirection::Left, KeyCode::A);
			move2keyMap.insert_or_assign(MoveDirection::Right, KeyCode::S);
			move2keyMap.insert_or_assign(MoveDirection::Back, KeyCode::D);
			move2keyMap.insert_or_assign(MoveDirection::Up, KeyCode::SPACE);
			move2keyMap.insert_or_assign(MoveDirection::Down, KeyCode::LEFT_SHIFT);
			move2keyMap.insert_or_assign(MoveDirection::TurnLeft, KeyCode::A);
			move2keyMap.insert_or_assign(MoveDirection::TurnRight, KeyCode::D);
		}
		void update() {
			front = front_init;
			pitch = glm::degrees(asin(glm::normalize(front_init).y));
			yaw = glm::degrees(asin(glm::normalize(front_init).z / cos(pitch)));
		}
	};

	DEFINE_OBJECT(MoveController);

	void MoveController::start() {
		pImpl->front_init = transform()->rotation() * glm::vec3(0.0, 0.0, -1.0);
		pImpl->rot_init = transform()->rotation();
		if (pImpl->rot_init == glm::quat {0, 0, 0, 0})
			pImpl->rot_init = glm::quat(glm::vec3(0.0, 0.0, -1.0));
		pImpl->update();
	}

	void MoveController::setUp(glm::vec3 value) { pImpl->up = value; }
	glm::vec3 MoveController::getUp() const { return pImpl->up; }

	void MoveController::setFront(glm::vec3 value) { pImpl->front = value; }
	glm::vec3 MoveController::getFront() const { return pImpl->front; }

	float MoveController::getSensitivity() const { return pImpl->sensitivity; }
	void MoveController::setSensitivity(float value) { pImpl->sensitivity = value; }

	MoveType MoveController::getMoveType() const { return pImpl->moveType; }
	void MoveController::setMoveType(MoveType value) { pImpl->moveType = value; }

	void MoveController::setSpeed(float value) { pImpl->speed = value; }
	float MoveController::getSpeed() const { return pImpl->speed; }

	const std::map<MoveDirection, KeyCode>& MoveController::getKeyMap() const { return pImpl->move2keyMap; }
	void MoveController::setKeyMap(std::map<MoveDirection, KeyCode> mapNew) { pImpl->move2keyMap = std::move(mapNew); }
	void MoveController::setKeyMap(MoveDirection md, KeyCode code) { pImpl->move2keyMap.insert_or_assign(md, code); }

	void MoveController::update() {
		//get basic position information
		float moveSpeed = Time::deltaTime() * pImpl->speed;
		auto objPos = transform()->position();
		glm::vec3 goFront;
		switch (pImpl->moveType)
		{
			case(MoveType::Flat):
				goFront = glm::vec3({pImpl->front.x, 0.0, pImpl->front.z});
				break;
			case(MoveType::Free):
				goFront = pImpl->front;
				break;
			default:
				break;
		}

		//update state
		if (Input::getMouseButtonDown(MouseButton::Middle) && !pImpl->mouseMidPressed) {
			pImpl->mouseMidPressed = true;
		}
		if (Input::getMouseButtonUp(MouseButton::Middle) && pImpl->mouseMidPressed) {
			pImpl->mouseMidPressed = false;
			//hide/unhide mouse pointer. Control by middle button
			if (Input::getMouseButtonUp(MouseButton::Middle)) {
				if (pImpl->hideMouse)
					pImpl->hideMouse = false;
				else
					pImpl->hideMouse = true;
			}
		}


		//move, control by keybord
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Front])) objPos += moveSpeed * goFront;
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Right])) objPos -= moveSpeed * goFront;
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Left])) objPos -= glm::normalize(glm::cross(goFront, pImpl->up)) * moveSpeed;
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Back])) objPos += glm::normalize(glm::cross(goFront, pImpl->up)) * moveSpeed;
		switch (pImpl->moveType)
		{
			case MoveType::Free:
				if (Input::getKey(pImpl->move2keyMap[MoveDirection::Up])) objPos += pImpl->up * moveSpeed;
				if (Input::getKey(pImpl->move2keyMap[MoveDirection::Down])) objPos -= pImpl->up * moveSpeed;
				break;
			case MoveType::Flat:
				break;
			default:
				break;
		}

		//direction control
		float xOffset = 0, yOffset = 0;
		if (pImpl->directController == DirectController::Mouse) {
			if (pImpl->hideMouse) {
				Cursor::setMode(CursorMode::Disabled);
				if (pImpl->firstMouse) {
					pImpl->mousePosLast = Input::mousePosition();
					pImpl->firstMouse = false;
				}
				glm::vec3 mousePosNow = Input::mousePosition();
				glm::vec3 posDelta = mousePosNow - pImpl->mousePosLast;
				pImpl->mousePosLast = mousePosNow;
				xOffset = posDelta.x * pImpl->sensitivity;
				yOffset = -posDelta.y * pImpl->sensitivity;
			} else {//control by left mouse button pressed
				Cursor::setMode(CursorMode::Normal);
				if (Input::getMouseButtonDown(MouseButton::Left) && !pImpl->mouseLeftPressed) {
					pImpl->mousePosLast = Input::mousePosition();
					pImpl->mouseLeftPressed = true;
				}
				if (Input::getMouseButtonUp(MouseButton::Left)) {
					pImpl->mouseLeftPressed = false;
				}
				if (Input::getMouseButtonDown(MouseButton::Left)) {
					glm::vec3 posNow = Input::mousePosition();
					glm::vec3 posDelta = posNow - pImpl->mousePosLast;
					pImpl->mousePosLast = posNow;
					xOffset = posDelta.x * pImpl->sensitivity;
					yOffset = -posDelta.y * pImpl->sensitivity;
				}
			}
		} else if (pImpl->directController == DirectController::KeyBoard) {
			if (Input::getKey(pImpl->move2keyMap[MoveDirection::TurnRight])) xOffset = 3;
			if (Input::getKey(pImpl->move2keyMap[MoveDirection::TurnLeft])) xOffset = -3;
		}
		pImpl->yaw += xOffset;
		pImpl->pitch += yOffset;

		if (pImpl->pitch > 89.0f)
			pImpl->pitch = 89.0f;
		if (pImpl->pitch < -89.0f)
			pImpl->pitch = -89.0f;

		glm::vec3 frontNew {
			cos(glm::radians(pImpl->yaw)) * cos(glm::radians(pImpl->pitch)),
			sin(glm::radians(pImpl->pitch)),
			sin(glm::radians(pImpl->yaw)) * cos(glm::radians(pImpl->pitch))
		};

		//change position and rotation
		pImpl->front = glm::normalize(frontNew);
		transform()->position(objPos);
		transform()->rotation(RotU2V(pImpl->front_init, pImpl->front) * pImpl->rot_init);

		auto cam = getComponent<Camera>();
		if (cam) {
			cam->setFieldOfView(std::clamp(cam->getFieldOfView() + Input::mouseScrollDelta().y, 1.0f, 90.0f));
		}
	}

	glm::quat RotU2V(glm::vec3 start, glm::vec3 dest) {
		start = normalize(start);
		dest = normalize(dest);
		float cosTheta = dot(start, dest);
		glm::vec3 rotationAxis;

		rotationAxis = cross(start, dest);
		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;
		glm::quat res(
			s * 0.5f,
			rotationAxis.x * invs,
			rotationAxis.y * invs,
			rotationAxis.z * invs
		);
		return res;
	}
}
