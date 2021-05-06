#pragma once

namespace otg {

	class MoveableCamera {

	public:
		virtual void moveLeft(float frameTime) {};
		virtual void moveRight(float frameTime) {};
		virtual void moveUp(float frameTime) {};
		virtual void moveDown(float frameTime) {};
		virtual void moveForward(float frameTime) {};
		virtual void moveBackwards(float frameTime) {};

		virtual void move(glm::vec2 offset) {};

	protected:
		float sensitivity;
		float movementSpeed;
	};
	
}