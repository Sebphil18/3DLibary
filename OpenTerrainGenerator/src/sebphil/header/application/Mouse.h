#pragma once
#include "glm/glm.hpp"
#include "application/Window.h"

namespace otg {

	class Mouse {

	public:
		Mouse(Window& window);

		void move();
		glm::vec2 getNewPos();
		void updatePosDelta(glm::dvec2 newPos);

		void resetScrollDelta();

		glm::ivec2 getScrollOffset() const ;
		std::int32_t getScrollOffsetY() const;

		glm::dvec2 getLastPos() const;
		glm::dvec2 getPosDelta() const;
		bool isPrimaryPressed();
		bool isButtonPressed(int button);

	private:
		GLFWwindow* windowHandle;
		glm::dvec2 lastPos, delta;
		glm::ivec2 lastScrollPos, scrollDelta;
		
	};

}