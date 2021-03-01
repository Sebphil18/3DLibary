#pragma once
#include <string>

namespace otg {
	
	struct Image {
		unsigned char* buffer;
		int width, height;
		int channels;
	};

	class ImageLoader : public Image {

	public:
		ImageLoader(const std::string& filePath) noexcept;
		ImageLoader(const ImageLoader& otherLoader) noexcept;
		ImageLoader(ImageLoader&& otherLoader) noexcept;
		
		ImageLoader& operator=(const ImageLoader& otherLoader) noexcept;
		ImageLoader& operator=(ImageLoader&& otherLoader) noexcept;

		~ImageLoader() noexcept;

	private:
		std::string filePath;

		void validateBuffer();

	};

}