#pragma once

namespace otg {

	struct Frame {
		unsigned long currentFrame;
		double elapsedTime, frameTime, frameRate;
	};

	class FrameClock {

	public:
		FrameClock();

		void update();

		double getFrameRate() const;
		double getFrameTime() const;
		double getElapsedTime() const;
		unsigned long getCurrentFrame() const;

	private:
		Frame frame;

		void updateFrame();

	};

}
