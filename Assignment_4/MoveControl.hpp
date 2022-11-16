#pragma once
#include<roamer_engine/display/Component.hpp>
#include<map>
#include<roamer_engine/input.hpp>

namespace qy::cg {
	enum class MoveType {
		Free,
		Flat,
		Jump,
	};

	enum class DirectController {
		Mouse,
		KeyBoard
	};

	enum class MoveDirection {
		Front, Left, Right, Back, Up, Down, Jump,TurnLeft, TurnRight
	};
	glm::quat RotU2V(glm::vec3 start, glm::vec3 dest);

	class MoveControl:public Component {
	private:
		DECL_PIMPL;
	public:
		DECL_OBJECT(MoveControl)
		void update();
		void init(glm::vec3 front_init);
		glm::vec3 getFront();
		void setFront(glm::vec3 value);
		void setUp(glm::vec3 value);
		glm::vec3 getUp();
		void setSensitivity(float value);
		float getSensitivity();
		void setMoveType(MoveType value);
		MoveType getMoveType();
		void setSpeed(float value);
		float getSpeed();
		void setJumpHeight(float value);
		float getJumpHeith();
		std::map<MoveDirection, KeyCode> getMove2CodeMap();
		void setMove2CodeMap(std::map<MoveDirection, KeyCode> value);
		void updateKeyMap(MoveDirection md, KeyCode code);
	};
}