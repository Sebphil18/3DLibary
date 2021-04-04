#pragma once
#include <string>
#include "globjects/TextureType.h"
#include "globjects/Texture.h"
#include "io/Image.h"
#include "globjects/GlObject.h"

namespace otg {

	class TextureImage : public Texture{

	public:
		TextureImage() noexcept;
		TextureImage(const std::string& filePath, TextureType type) noexcept;
		TextureImage(const TextureImage& otherTex) noexcept;
		TextureImage(TextureImage&& otherTex) noexcept;

		TextureImage& operator=(const TextureImage& otherTex) noexcept;
		TextureImage& operator=(TextureImage&& otherTex) noexcept;

		~TextureImage() noexcept;

		void setImage(const std::string& filePath, TextureType type);

		TextureType getType() const;
		unsigned char* getBufferPtr() const;

	private:
		std::string filePath;

		void loadTexture();
		void tryLoadImg();
		void loadImg();

		void setSize(std::int32_t width, std::int32_t height) = delete;

	};

}
