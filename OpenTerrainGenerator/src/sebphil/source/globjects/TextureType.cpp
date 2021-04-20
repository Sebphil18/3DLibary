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

	TextureType TextureTypes::ofAiTextureType(aiTextureType aiType) {
		
		switch (aiType) {

		case aiTextureType_BASE_COLOR:
			return TextureType::Albedo;

		case aiTextureType_DIFFUSE:
			return TextureType::Albedo;

		case aiTextureType_DIFFUSE_ROUGHNESS:
			return TextureType::Roughness;

		case aiTextureType_SPECULAR:
			return TextureType::Roughness;

		case aiTextureType_METALNESS:
			return TextureType::Metallic;

		case aiTextureType_AMBIENT:
			return TextureType::Occlusion;

		case aiTextureType_DISPLACEMENT:
			return TextureType::Displacement;

		case aiTextureType_NORMALS:
			return TextureType::Normal;

		default:
			return TextureType::RGBA;
		}
		
	}

	std::uint32_t TextureTypes::getGlType(TextureType type) {

		switch (type) {

		case TextureType::Multisample:
			return GL_TEXTURE_2D_MULTISAMPLE;

		case TextureType::CubeMapArray:
			return GL_TEXTURE_CUBE_MAP_ARRAY;

		default:
			return GL_TEXTURE_2D;
		}
	}

	std::uint32_t TextureTypes::getGlFormat(TextureType type) {

		switch (type) {

		case TextureType::RGB:
			return GL_RGB16;

		case TextureType::RGBFloat:
			return GL_RGB16F;

		case TextureType::CubeMapArray:
			return GL_RGB16F;

		case TextureType::Albedo:
			return GL_RGBA16F;

		case TextureType::RGBAFloat:
			return GL_RGBA16F;

		case TextureType::Multisample:
			return GL_RGBA16F;

		case TextureType::ColorAttachment:
			return GL_RGBA16F;

		case TextureType::SRGBA:
			return GL_SRGB8_ALPHA8;

		case TextureType::DepthStencilAttachment:
			return GL_DEPTH24_STENCIL8;

		default:
			return GL_RGBA16;
		}
	}

	std::uint32_t TextureTypes::getGlDataType(TextureType type) {

		switch (type) {

		case TextureType::RGBFloat:
			return GL_FLOAT;

		case TextureType::RGBAFloat:
			return GL_FLOAT;

		case TextureType::CubeMapArray:
			return GL_FLOAT;

		default:
			return GL_UNSIGNED_BYTE;
		}

	}

}