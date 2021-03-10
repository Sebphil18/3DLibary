#pragma once
#include <cstdint>

namespace otg {

	class Application {

	public:
		Application();
		~Application();

	private:
		static std::uint32_t instances;
		
		// disable copying, moving and/or assigning to this singleton (only one instance of this class should be created)
		Application(Application& otherApp) = delete;
		Application(Application&& otherApp) = delete;
		Application& operator=(Application& otherApp) = delete;
		Application& operator=(Application&& otherApp) = delete;

		static void instantiate();
		static void init();
		static void errorCallback(int errorCode, const char* description);

	};

}