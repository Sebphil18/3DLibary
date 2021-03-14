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

	std::uint32_t TextureTypes::getGlType(TextureType type) {

		switch (type) {

		case TextureType::Multisample:
			return GL_TEXTURE_2D_MULTISAMPLE;

		default:
			return GL_TEXTURE_2D;
		}
	}

	TextureType TextureTypes::getType() const {
		return type;
	}

}