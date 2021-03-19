#pragma once
#include <cstdint>

namespace otg {

	class TextureTypes {

	public:
		enum class TextureType {
			None, RGBA, SRGBA, Multisample, ColorAttachment, DepthStencilAttachment, RGBAFloat, 
			Albedo, Height, Roughness, Metalness, Occlussion
		};

		TextureTypes() noexcept;
		TextureTypes(TextureType type) noexcept;

		TextureType getType() const;

		static std::uint32_t getGlType(TextureType type);
		static std::uint32_t getGlFormat(TextureType type);
		static std::uint32_t getGlDataType(TextureType type);

	protected:
		TextureType type;

	};

	typedef TextureTypes::TextureType TextureType;

}