#include "io/ImageLoader.h"
#include <iostream>
#include <memory>
#include "exceptions/ImageLoadException.h"
#include "stb/stbimage.h"

namespace otg {

	ImageLoader::ImageLoader(const std::string& filePath) noexcept :
		filePath(filePath)
	{
		
		buffer = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		validateBuffer();
	}

	ImageLoader::ImageLoader(const ImageLoader& otherLoader) noexcept :
		filePath(otherLoader.filePath)
	{
		buffer = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
		validateBuffer();
	}

	ImageLoader& ImageLoader::operator=(const ImageLoader& otherLoader)	noexcept {

		filePath = otherLoader.filePath;
		buffer = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

		validateBuffer();

		return *this;
	}

	void otg::ImageLoader::validateBuffer() {

		try {

			if (!buffer)
				throw ImageLoaderException(filePath);

		} catch (const ImageLoaderException& exception) {
			std::cout << exception.what() << "\n";
		}

	}

	ImageLoader::ImageLoader(ImageLoader&& otherLoader) noexcept :
		filePath(std::move(otherLoader.filePath)) {
		width = std::move(otherLoader.width);
		height = std::move(otherLoader.height);
		channels = std::move(otherLoader.channels);

		buffer = std::move(otherLoader.buffer);
	}

	ImageLoader& ImageLoader::operator=(ImageLoader&& otherLoader) noexcept {

		filePath = std::move(otherLoader.filePath);
		width = std::move(otherLoader.width);
		height = std::move(otherLoader.height);
		channels = std::move(otherLoader.channels);

		buffer = std::move(otherLoader.buffer);

		return *this;
	}

	ImageLoader::~ImageLoader() noexcept {
		stbi_image_free(buffer);
	}

}