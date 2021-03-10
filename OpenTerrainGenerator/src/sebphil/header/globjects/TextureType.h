#pragma once
#include <cstdint>

namespace otg {

	class TextureTypes {

	public:
		enum class TextureType {
			None, Albedo, Roughness, Occlussion, Metalness, Height, Multisample
		};

		static std::uint32_t getGlType(TextureType type);

	};

	typedef TextureTypes::TextureType TextureType;

}