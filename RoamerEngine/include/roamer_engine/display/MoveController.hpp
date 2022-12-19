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

	class MoveController : public Component {
	private:
		DECL_PIMPL;
	public:
		DECL_OBJECT(MoveController);

		void update() override;

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