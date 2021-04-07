#pragma once
#include <cstdint>
#include "assimp/scene.h"

namespace otg {

	class TextureTypes {

	public:
		enum class TextureType {
			None, RGB, RGBA, SRGBA, Multisample, ColorAttachment, DepthStencilAttachment, RGBAFloat, 
			Albedo, Displacement, Roughness, Metallic, Occlussion, Ambient, Normal
		};

		TextureTypes() noexcept;
		TextureTypes(TextureType type) noexcept;

		TextureType getType() const;

		static TextureType ofAiTextureType(aiTextureType aiType);

		static std::uint32_t getGlType(TextureType type);
		static std::uint32_t getGlFormat(TextureType type);
		static std::uint32_t getGlDataType(TextureType type);

	protected:
		TextureType type;

	};

	typedef TextureTypes::TextureType TextureType;

}