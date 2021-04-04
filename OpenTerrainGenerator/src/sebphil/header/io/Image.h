#pragma once
#include <string>

namespace otg {

	struct Image {
		unsigned char* buffer = nullptr;
		int width, height, channels;
	};

}