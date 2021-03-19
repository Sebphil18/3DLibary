#include "globjects/TextureType.h"
#include "glad/glad.h"

namespace otg {

	TextureTypes::TextureTypes() noexcept :
		type(TextureType::None)
	{
	}

	TextureTypes::TextureTypes(TextureType type) noexcept : 
		type(type)
	{
	}

	TextureType TextureTypes::getType() const {
		return type;
	}

	std::uint32_t TextureTypes::getGlType(TextureType type) {

		switch (type) {

		case TextureType::Multisample:
			return GL_TEXTURE_2D_MULTISAMPLE;

		default:
			return GL_TEXTURE_2D;
		}
	}

	std::uint32_t TextureTypes::getGlFormat(TextureType type) {

		switch (type) {

		case TextureType::Albedo:
			return GL_SRGB8_ALPHA8;

		case TextureType::SRGBA:
			return GL_SRGB8_ALPHA8;

		case TextureType::RGBAFloat:
			return GL_RGBA16F;

		case TextureType::Multisample:
			return GL_RGBA16F;

		case TextureType::DepthStencilAttachment:
			return GL_DEPTH24_STENCIL8;

		default:
			return GL_RGBA16;
		}
	}

	std::uint32_t TextureTypes::getGlDataType(TextureType type) {

		switch (type) {

		case TextureType::RGBAFloat:
			return GL_FLOAT;

		default:
			return GL_UNSIGNED_BYTE;
		}

	}

}