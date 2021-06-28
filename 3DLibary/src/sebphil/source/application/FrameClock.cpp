#include "application/FrameClock.h"
#include "GLFW/glfw3.h"

glib::FrameClock::FrameClock() {

	frame = { 0, 0, 0, 0 };
}

void glib::FrameClock::tick() {

	updateFrame();
}

void glib::FrameClock::updateFrame() {

	double currentTime = glfwGetTime();

	frame.frameTime = currentTime - frame.elapsedTime;

	if(frame.frameTime != 0)
		frame.frameRate = 1 / frame.frameTime;

	frame.elapsedTime = currentTime;
	frame.currentFrame++;
}

double glib::FrameClock::getFrameRate() const {
	return frame.frameRate;
}

double glib::FrameClock::getFrameTime() const {
	return frame.frameTime;
}

double glib::FrameClock::getElapsedTime() const {
	return frame.elapsedTime;
}

unsigned long glib::FrameClock::getCurrentFrame() const {
	return frame.currentFrame;
}
