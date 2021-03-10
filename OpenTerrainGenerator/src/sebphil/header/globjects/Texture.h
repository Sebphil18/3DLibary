#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"
#include "io/Image.h"

namespace otg {

	// TODO: implement TexutreType::AttachmentRGB/RGBA
	class Texture : public GlObject, public TextureTypes {

	public:
		Texture() noexcept;
		Texture(std::int32_t width, std::int32_t height, TextureType type) noexcept;
		Texture(const Image& img, TextureType type) noexcept;
		Texture(const Texture& otherTex) noexcept;
		Texture(Texture&& otherTex) noexcept;
		
		Texture& operator=(const Texture& otherTex) noexcept;
		Texture& operator=(Texture&& otherTex) noexcept;

		~Texture() noexcept;

		void generateMipmap();

		TextureType getType() const;

	protected:
		Image img;
		TextureType type;

		void createTexture();
		void specifyStorage();
		void specifySubImg();

	};

}