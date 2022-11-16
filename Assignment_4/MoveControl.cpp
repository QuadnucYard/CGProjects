#include "MoveControl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <roamer_engine/input.hpp>
#include <roamer_engine/Time.hpp>
#include <roamer_engine/display/Camera.hpp>
#include <map>
#include <cmath>

namespace qy::cg {
	struct MoveControl::Impl {
		glm::vec3 front_init{ 0.0, 0.0, -1.0 };
		glm::vec3 front;
		glm::vec3 up{ 0, 1, 0 };
		glm::vec3 mousePosLast = { 0.0f, 0.0f, 0.0f };
		float sensitivity = 0.1f;
		float pitch;
		float yaw;
		bool firstMouse = true;
		MoveType moveType = MoveType::Flat;
		DirectController directController = DirectController::Mouse;
		float speed = 3;
		float jumpHeight = 0.5;
		std::map<MoveDirection, KeyCode> move2keyMap;

		Impl() {
			front = front_init;
			pitch = glm::degrees(asin(glm::normalize(front_init).y));
			yaw = glm::degrees(asin(glm::normalize(front_init).z / cos(pitch)));
			move2keyMap.insert_or_assign(MoveDirection::Front, KeyCode::W);
			move2keyMap.insert_or_assign(MoveDirection::Left, KeyCode::A);
			move2keyMap.insert_or_assign(MoveDirection::Right, KeyCode::S);
			move2keyMap.insert_or_assign(MoveDirection::Back, KeyCode::D);
			move2keyMap.insert_or_assign(MoveDirection::Up, KeyCode::LEFT_SHIFT);
			move2keyMap.insert_or_assign(MoveDirection::Down, KeyCode::SPACE);
			move2keyMap.insert_or_assign(MoveDirection::Jump, KeyCode::SPACE);
			move2keyMap.insert_or_assign(MoveDirection::TurnLeft, KeyCode::A);
			move2keyMap.insert_or_assign(MoveDirection::TurnRight, KeyCode::D);
		}
	};

	DEFINE_OBJECT(MoveControl);

	void MoveControl::setFrontInit(glm::vec3 value) { pImpl->front_init = value; }
	glm::vec3 MoveControl::getFrontInit() { return pImpl->front_init; }
	void  MoveControl::setUp(glm::vec3 value) { pImpl->up = value; }
	glm::vec3  MoveControl::getUp() { return pImpl->up; }
	void  MoveControl::setSensitivity(float value) { pImpl->sensitivity = value; }
	float  MoveControl::getSensitivity() { return pImpl->sensitivity; }
	void  MoveControl::setMoveType(MoveType value) { pImpl->moveType = value; }
	MoveType  MoveControl::getMoveType() { return pImpl->moveType; }
	void  MoveControl::setSpeed(float value) { pImpl->speed = value; }
	float  MoveControl::getSpeed() { return pImpl->speed; }
	void  MoveControl::setJumpHeight(float value) { pImpl->jumpHeight = value; }
	float  MoveControl::getJumpHeith() { return pImpl->jumpHeight; }
	std::map<MoveDirection, KeyCode>  MoveControl::getMove2CodeMap() { return pImpl->move2keyMap; }
	void  MoveControl::setMove2CodeMap(std::map<MoveDirection, KeyCode> value) { pImpl->move2keyMap = value; }
	void  MoveControl::updateKeyMap(MoveDirection md, KeyCode code) { pImpl->move2keyMap.insert_or_assign(md, code); }
	
	void MoveControl::update() {
		float moveSpeed = (float)Time::deltaTime() * pImpl->speed;
		auto objPos = transform()->position();
		glm::vec3 goFront;
		switch (pImpl->moveType)
		{
		case(MoveType::Flat):
			goFront = glm::vec3({ pImpl->front.x, 0.0, pImpl->front.z });
			break;
		case(MoveType::Free):
			goFront = pImpl->front;
		case(MoveType::Jump):
			goFront = glm::vec3({ pImpl->front.x, 0.0, pImpl->front.z });
			break;
		default:
			break;
		}
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
		case MoveType::Jump:
			//TODO
			break;
		default:
			break;
		}

		float xOffset, yOffset;
		if (pImpl->directController == DirectController::Mouse) {
			if (pImpl->firstMouse) {
				pImpl->mousePosLast = Input::mousePosition();
				pImpl->firstMouse = false;
			}
			glm::vec3 mousePosNow = Input::mousePosition();
			glm::vec3 posDelta = mousePosNow - pImpl->mousePosLast;
			pImpl->mousePosLast = mousePosNow;
			xOffset = posDelta.x * pImpl->sensitivity;
			yOffset = -posDelta.y * pImpl->sensitivity;
		}
		else if (pImpl->directController == DirectController::KeyBoard) {
			if (Input::getKey(pImpl->move2keyMap[MoveDirection::TurnRight])) xOffset = 3;
			if (Input::getKey(pImpl->move2keyMap[MoveDirection::TurnLeft])) xOffset = -3;
		}

		pImpl->yaw += xOffset;
		pImpl->pitch += yOffset;

		if (pImpl->pitch > 89.0f)
			pImpl->pitch = 89.0f;
		if (pImpl->pitch < -89.0f)
			pImpl->pitch = -89.0f;

		glm::vec3 frontNew;
		frontNew.x = cos(glm::radians(pImpl->yaw)) * cos(glm::radians(pImpl->pitch));
		frontNew.y = sin(glm::radians(pImpl->pitch));
		frontNew.z = sin(glm::radians(pImpl->yaw)) * cos(glm::radians(pImpl->pitch));
		
		pImpl->front = glm::normalize(frontNew);
		transform()->position(objPos);
		transform()->rotation(RotU2V(pImpl->front_init, pImpl->front));
		auto cam = getComponent<Camera>();
		if (cam) {
			cam->setFieldOfView(std::clamp(cam->getFieldOfView() + Input::mouseScrollDelta().y, 1.0f, 80.0f));
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