#pragma once
#include <string>

namespace otg {

	struct Image {
		unsigned char* buffer;
		int width, height, channels;
	};

}