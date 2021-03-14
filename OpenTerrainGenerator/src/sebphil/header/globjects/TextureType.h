#pragma once
#include <cstdint>

namespace otg {

	class TextureTypes {

	public:
		enum class TextureType {
			None, Albedo, Roughness, Occlussion, Metalness, Height, Multisample, ColorAttachment, DepthStencilAttachment
		};

		TextureTypes() noexcept;
		TextureTypes(TextureType type) noexcept;

		static std::uint32_t getGlType(TextureType type);

		TextureType getType() const;

	protected:
		TextureType type;

	};

	typedef TextureTypes::TextureType TextureType;

}