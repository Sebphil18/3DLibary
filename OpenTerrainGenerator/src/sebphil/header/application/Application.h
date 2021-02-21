#pragma once
#include <cstdint>

namespace otg {

	class Application {

	public:
		Application();
		~Application();

	private:
		static std::uint32_t instances;
		
		// disable copying, moving or assigning to this singleton
		Application(Application& otherApp) = delete;
		Application(Application&& otherApp) = delete;
		Application& operator=(Application& otherApp) = delete;

		static void instantiate();
		static void initialize();
		static void errorCallback(int errorCode, const char* description);

	};

}