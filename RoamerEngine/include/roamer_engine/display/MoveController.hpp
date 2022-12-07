#pragma once
#include <roamer_engine/display/Component.hpp>
#include <roamer_engine/input.hpp>

namespace qy::cg {

	enum class MoveType {
		Free,
		Flat,
	};

	enum class DirectController {
		Mouse,
		KeyBoard
	};

	enum class MoveDirection {
		Front, Left, Right, Back, Up, Down, Jump, TurnLeft, TurnRight
	};

	glm::quat RotU2V(glm::vec3 start, glm::vec3 dest);

	class MoveController : public Component {
	private:
		DECL_PIMPL;
	public:
		DECL_OBJECT(MoveController);

		void start();
		void update();

		glm::vec3 getFront() const;
		void setFront(glm::vec3 value);
		glm::vec3 getUp() const;
		void setUp(glm::vec3 value);
		float getSensitivity() const;
		void setSensitivity(float value);
		MoveType getMoveType() const;
		void setMoveType(MoveType value);
		float getSpeed() const;
		void setSpeed(float value);

		const std::map<MoveDirection, KeyCode>& getKeyMap() const;
		void setKeyMap(std::map<MoveDirection, KeyCode> mapNew);
		void setKeyMap(MoveDirection md, KeyCode code);
	};

}