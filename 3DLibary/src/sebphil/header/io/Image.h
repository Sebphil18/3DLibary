#pragma once
#include <string>

namespace glib {

	struct Image {
		unsigned char* buffer = nullptr;
		std::int32_t width, height, channels;
	};

	struct ImageFloat {
		float* buffer = nullptr;
		std::int32_t width, height, channels;
	};

	struct ImageFormat {
		std::int32_t width, height;
		std::int32_t desiredChannels = 0;
	};

}