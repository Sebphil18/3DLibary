#pragma once
#include <cstdint>
#include "glm/glm.hpp"

namespace otg {

	class Camera {

	public:
		Camera();
		Camera(int width, int height);
		Camera(int width, int height, glm::vec3 position);

		void setPosition(glm::vec3 position);
		void setTarget(glm::vec3 target);
		void setFov(float fov);
		void setWidth(int width);
		void setHeight(int height);
		void setSize(int width, int height);

		glm::vec3 getPosition() const;
		glm::vec3 getTarget() const;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix() const;

	private:
		float fov, near, far;
		int width, height;

		glm::vec3 position, target, up, right;
		glm::mat4 view, projection;

		void init();

		void constructAxis();
		void updateView();
		void updateProjection();

	};

}