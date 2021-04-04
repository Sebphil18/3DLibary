#include "application/FrameClock.h"
#include "GLFW/glfw3.h"

otg::FrameClock::FrameClock() {

	frame = { 0, 0, 0, 0 };
}

void otg::FrameClock::tick() {

	updateFrame();
}

void otg::FrameClock::updateFrame() {

	double currentTime = glfwGetTime();

	frame.frameTime = currentTime - frame.elapsedTime;

	if(frame.frameTime != 0)
		frame.frameRate = 1 / frame.frameTime;

	frame.elapsedTime = currentTime;
	frame.currentFrame++;
}

double otg::FrameClock::getFrameRate() const {
	return frame.frameRate;
}

double otg::FrameClock::getFrameTime() const {
	return frame.frameTime;
}

double otg::FrameClock::getElapsedTime() const {
	return frame.elapsedTime;
}

unsigned long otg::FrameClock::getCurrentFrame() const {
	return frame.currentFrame;
}
