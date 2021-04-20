#pragma once
#include <string>

namespace otg {

	struct Image {
		unsigned char* buffer = nullptr;
		std::int32_t width, height, channels;
	};

	struct ImageFloat {
		float* buffer = nullptr;
		std::int32_t width, height, channels;
	};

}