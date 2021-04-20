#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"
#include "io/Image.h"

namespace otg {

	class MultisampleTexture : public GlObject, public TextureTypes {

	public:
		MultisampleTexture() noexcept;
		MultisampleTexture(std::int32_t width, std::int32_t height) noexcept;
		MultisampleTexture(std::int32_t width, std::int32_t height, std::uint32_t samples) noexcept;
		MultisampleTexture(const MultisampleTexture& other) noexcept;
		MultisampleTexture(MultisampleTexture&& other) noexcept;

		MultisampleTexture& operator=(const MultisampleTexture& other) noexcept;
		MultisampleTexture& operator=(MultisampleTexture&& other) noexcept;

		~MultisampleTexture() noexcept;

		void setSize(std::int32_t width, std::int32_t height);

	private:
		std::uint32_t samples;
		Image img;

		void initTexture();
		void createTexture();
		void specifyStorage();
		void deleteTexture();

	};

}
