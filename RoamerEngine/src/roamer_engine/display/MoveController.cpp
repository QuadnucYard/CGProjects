#include "roamer_engine/display/MoveController.hpp"
#include <roamer_engine/input.hpp>
#include <roamer_engine/Time.hpp>
#include <roamer_engine/display/Camera.hpp>
#include <roamer_engine/display/Transform.hpp>
#include <roamer_engine/Application.hpp>

namespace qy::cg {

	struct MoveController::Impl {
		glm::vec3 mousePosLast = {0.0f, 0.0f, 0.0f};
		float sensitivity = 1.0f;
		bool firstMouse = true;
		MoveType moveType = MoveType::Flat;
		DirectController directController = DirectController::Mouse;
		float speed = 3;
		std::map<MoveDirection, KeyCode> move2keyMap;
		bool hideMouse = true;
		bool mouseLeftPressed = false;
		bool mouseMidPressed = false;

		Impl() {
			move2keyMap.insert_or_assign(MoveDirection::Front, KeyCode::W);
			move2keyMap.insert_or_assign(MoveDirection::Left, KeyCode::A);
			move2keyMap.insert_or_assign(MoveDirection::Right, KeyCode::S);
			move2keyMap.insert_or_assign(MoveDirection::Back, KeyCode::D);
			move2keyMap.insert_or_assign(MoveDirection::Up, KeyCode::SPACE);
			move2keyMap.insert_or_assign(MoveDirection::Down, KeyCode::LEFT_SHIFT);
			move2keyMap.insert_or_assign(MoveDirection::TurnLeft, KeyCode::A);
			move2keyMap.insert_or_assign(MoveDirection::TurnRight, KeyCode::D);
		}
	};

	DEFINE_OBJECT(MoveController);

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
		auto rot = transform()->rotation();
		auto front = transform()->rotation() * vec3_back;
		auto up = transform()->rotation() * vec3_up;

		glm::vec3 goFront;
		switch (pImpl->moveType)
		{
			case(MoveType::Flat):
				goFront = glm::vec3({front.x, 0.0, front.z});
				break;
			case(MoveType::Free):
				goFront = front ;
				break;
			default:
				break;
		}

		//update state
		if (Input::getKeyDown(KeyCode::TAB) && !pImpl->mouseMidPressed) {
			pImpl->mouseMidPressed = true;
		}
		if (Input::getKeyUp(KeyCode::TAB) && pImpl->mouseMidPressed) {
			pImpl->mouseMidPressed = false;
			//hide/unhide mouse pointer. Control by middle button
			if (Input::getKeyUp(KeyCode::TAB)) {
				if (pImpl->hideMouse)
					pImpl->hideMouse = false;
				else
					pImpl->hideMouse = true;
			}
		}


		//move, control by keybord
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Front])) objPos += moveSpeed * goFront;
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Right])) objPos -= moveSpeed * goFront;
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Left])) objPos -= glm::normalize(glm::cross(goFront, up)) * moveSpeed;
		if (Input::getKey(pImpl->move2keyMap[MoveDirection::Back])) objPos += glm::normalize(glm::cross(goFront, up)) * moveSpeed;
		switch (pImpl->moveType)
		{
			case MoveType::Free:
				if (Input::getKey(pImpl->move2keyMap[MoveDirection::Up])) objPos += up * moveSpeed;
				if (Input::getKey(pImpl->move2keyMap[MoveDirection::Down])) objPos -= up * moveSpeed;
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
				xOffset = posDelta.x;
				yOffset = -posDelta.y;
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
					xOffset = posDelta.x;
					yOffset = -posDelta.y;
				}
			}
		} else if (pImpl->directController == DirectController::KeyBoard) {
			if (Input::getKey(pImpl->move2keyMap[MoveDirection::TurnRight])) xOffset = 3;
			if (Input::getKey(pImpl->move2keyMap[MoveDirection::TurnLeft])) xOffset = -3;
		}

		const float pi = std::numbers::pi_v<float>;
		auto yaw = -xOffset / 1800 * pi * pImpl->sensitivity;
		glm::quat rotYaw = {glm::cos(yaw), glm::sin(yaw)*vec3_up};
		auto rotNew1 = glm::normalize(rotYaw * rot);
		front = rotNew1 * vec3_back;

		auto v = glm::normalize(glm::cross(front, vec3_up));
		auto pitch = yOffset / 1800 * pi * pImpl->sensitivity;
		glm::quat rotPitch = glm::quat {cos(pitch), sin(pitch) * v};
		auto rotNew2 = glm::normalize(rotPitch * rotYaw * rot);
		if ((rotNew2 * vec3_back).y > 0.99 || (rotNew2 * vec3_back).y <- 0.99)
			rotNew2 = rotYaw * rot;

		transform()->position(objPos);
		transform()->rotation(rotNew2);

		auto cam = getComponent<Camera>();
		if (cam) {
			cam->setFieldOfView(std::clamp(cam->getFieldOfView() + Input::mouseScrollDelta().y, 1.0f, 90.0f));
		}
	}
}
