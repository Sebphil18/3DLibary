#include "pch.h"
#include <thread>
#include <chrono>
#include <iostream>
#include "GLFW/glfw3.h"
#include "../OpenTerrainGenerator/src/sebphil/header/application/FrameClock.h"

class FrameClockTest : public ::testing::Test {

public:
	FrameClockTest() {
		glfwInit();
	}

	~FrameClockTest() {
		glfwTerminate();
	}

protected:
	otg::FrameClock clock;

};

double whenDeltaToReferenceTimeIsTaken(const otg::FrameClock& clock);
void thenSquareOfDeltaShouldBeSmall(double delta);

void thenElapsedTimeShouldEqualFrameTime(const otg::FrameClock& clock);

double whenOneOverFrameTimeIsTaken(const otg::FrameClock& clock);
void thenOneOverFrameTimeShouldEqualFrameRate(double reference, const otg::FrameClock& clock);

void givenClock(otg::FrameClock& clock);

void whenClockTicks(unsigned int ticks, otg::FrameClock& clock);
void thenCurrentFrameShouldEqualTicks(unsigned int ticks, const otg::FrameClock& clock);

TEST_F(FrameClockTest, testElapsedTime) {

	givenClock(clock);
	
	double delta = whenDeltaToReferenceTimeIsTaken(clock);

	thenSquareOfDeltaShouldBeSmall(delta);
}

double whenDeltaToReferenceTimeIsTaken(const otg::FrameClock& clock) {

	double clockTime = clock.getElapsedTime();
	double referenceDuration = glfwGetTime();

	return referenceDuration - clockTime;
}

void thenSquareOfDeltaShouldBeSmall(double delta) {

	double error = delta * delta;
	ASSERT_LE(error, 1e-5);
}

TEST_F(FrameClockTest, testFrameTime) {

	givenClock(clock);

	thenElapsedTimeShouldEqualFrameTime(clock);
}

void thenElapsedTimeShouldEqualFrameTime(const otg::FrameClock& clock) {

	double elapsedTime = clock.getElapsedTime();
	double frameTime = clock.getFrameTime();

	ASSERT_DOUBLE_EQ(elapsedTime, frameTime);
}

TEST_F(FrameClockTest, testFrameRate) {

	givenClock(clock);

	double reference = whenOneOverFrameTimeIsTaken(clock);

	thenOneOverFrameTimeShouldEqualFrameRate(reference, clock);
}

void givenClock(otg::FrameClock& clock) {
	clock.update();
}

double whenOneOverFrameTimeIsTaken(const otg::FrameClock& clock) {

	double frametime = clock.getFrameTime();
	
	return 1 / frametime;
}

void thenOneOverFrameTimeShouldEqualFrameRate(double reference, const otg::FrameClock& clock) {
	
	double frameRate = clock.getFrameRate();

	ASSERT_DOUBLE_EQ(reference, frameRate);
}

TEST_F(FrameClockTest, testCurrentFrame) {

	int ticks = 14;

	whenClockTicks(ticks, clock);

	thenCurrentFrameShouldEqualTicks(ticks, clock);
}

void whenClockTicks(unsigned int ticks, otg::FrameClock& clock) {

	for (int i = 0; i < ticks; i++)
		clock.update();

}

void thenCurrentFrameShouldEqualTicks(unsigned int ticks, const otg::FrameClock& clock) {

	unsigned long currentFrame = clock.getCurrentFrame();

	ASSERT_EQ(ticks, currentFrame);
}

TEST_F(FrameClockTest, testInitialState) {

	ASSERT_DOUBLE_EQ(clock.getCurrentFrame(), 0);
	ASSERT_DOUBLE_EQ(clock.getElapsedTime(), 0);
	ASSERT_DOUBLE_EQ(clock.getFrameRate(), 0);
	ASSERT_DOUBLE_EQ(clock.getElapsedTime(), 0);
}