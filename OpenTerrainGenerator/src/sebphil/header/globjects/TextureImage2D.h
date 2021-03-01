#pragma once
#include "globjects/GlObject.h"
#include "io/ImageLoader.h"

namespace otg {

	class TextureImage2D : public GlObject {

	public:
		TextureImage2D(const Image& image, std::uint32_t textureUnit);

		void setImage(const Image& image);

	private:
		Image image;

		void createTexture(std::uint32_t textureUnit);
		void setParameter();
		void setStorage();

	};

}