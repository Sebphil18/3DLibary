#pragma once
#include <string>
#include "io/Image.h"
#include "globjects/GlObject.h"

namespace otg {

	class HDRTexture : public GlObject {

	public:
		HDRTexture(const std::string& filePath) noexcept;
		HDRTexture(const HDRTexture& other) noexcept;
		HDRTexture(HDRTexture&& other) noexcept;

		HDRTexture& operator=(const HDRTexture& other) noexcept;
		HDRTexture& operator=(HDRTexture&& other) noexcept;

		~HDRTexture() noexcept;

		void bindToUnit(std::uint32_t unit);

	private:
		std::string filePath;
		ImageFloat img;

		void init();

		void tryLoadImage();
		void loadImage();

		void createTexture();
		void specifyStorage();
		void fillStorage();

		void deleteTexture();
	};

}