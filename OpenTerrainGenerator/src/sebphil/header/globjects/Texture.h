#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"
#include "io/Image.h"

namespace otg {

	struct DeferredTexture {
		std::string filePath;
		TextureType type;
	};

	class Texture : public GlObject, public TextureTypes {

	public:
		Texture() noexcept;
		Texture(TextureType type) noexcept;
		Texture(std::int32_t width, std::int32_t height, TextureType type) noexcept;
		Texture(const Image& img, TextureType type) noexcept;
		Texture(const Texture& otherTex) noexcept;
		Texture(Texture&& otherTex) noexcept;
		
		Texture& operator=(const Texture& otherTex) noexcept;
		Texture& operator=(Texture&& otherTex) noexcept;

		~Texture() noexcept;

		void generateMipmap();

		void setSize(std::int32_t width, std::int32_t height);

		void bindToUnit(std::uint32_t unit);

		std::int32_t getWidth() const;
		std::int32_t getHeight() const;

	protected:
		Image img;

		void initTexture();
		void createTexture();
		void specifyStorage(std::uint32_t mipmapLevels = 1);
		void specifySubImg();
		void deleteTexture();

	};

}