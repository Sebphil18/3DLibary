#pragma once
#include <string>
#include "modelstructure/TextureType.h"
#include "io/Image.h"
#include "globjects/GlObject.h"

namespace otg {

	class TextureImage : public GlObject {

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
		Image img;
		otg::TextureType type;
		std::string filePath;

		void init();
		void createTexture();
		void setTextureData();
		void setTextureParams();
		void generateMipmap();


	};

}
