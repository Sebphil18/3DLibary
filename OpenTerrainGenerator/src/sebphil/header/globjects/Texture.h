#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"
#include "io/Image.h"

namespace otg {

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

	protected:
		Image img;

		void createTexture();
		virtual void specifyStorage();
		void specifySubImg();

	};

}