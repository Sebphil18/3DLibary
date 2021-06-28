#pragma once
#include <cstdint>
#include "glm/glm.hpp"

namespace glib {

	class Camera {

	public:
		Camera();
		Camera(glm::ivec2 dimensions);
		Camera(glm::ivec2 dimensions, glm::vec3 position);

		virtual void setPosition(glm::vec3 position);
		void setTarget(glm::vec3 target);

		void setWidth(int width);
		void setHeight(int height);
		void setSize(int width, int height);
		void setSize(glm::ivec2 dimensions);

		void setFov(float fov);
		void setMovementSpeed(float movementSpeed);
		void setSensitivity(float sensitivity);

		virtual void move(glm::vec2 offset) {};
		virtual void moveLeft(float frameTime);
		virtual void moveRight(float frameTime);
		virtual void moveDown(float frameTime);
		virtual void moveUp(float frameTime);
		virtual void moveForward(float frameTime);
		virtual void moveBackwards(float frameTime);

		glm::vec3 getPosition() const;
		glm::vec3 getTarget() const;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

	protected:
		glm::vec3 position, target, up, right;
		glm::mat4 view, projection;
		float movementSpeed, sensitivity;

		virtual void updateViewMatrix();
		virtual void constructAxis();
		void updateProjection();

	private:
		float fov, near, far;
		glm::ivec2 dimensions;

		void init();

	};

}