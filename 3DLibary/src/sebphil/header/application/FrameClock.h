#pragma once

namespace glib {

	struct Frame {
		unsigned long currentFrame;
		double elapsedTime, frameTime, frameRate;
	};

	class FrameClock {

	public:
		FrameClock();

		void tick();

		double getFrameRate() const;
		double getFrameTime() const;
		double getElapsedTime() const;
		unsigned long getCurrentFrame() const;

	private:
		Frame frame;

		void updateFrame();

	};

}
