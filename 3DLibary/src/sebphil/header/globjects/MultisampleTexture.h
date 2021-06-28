#pragma once
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"
#include "io/Image.h"

namespace glib {

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

		/// <summary>
		/// This function will resize this texture, 
		/// i.e. the original TEXTURE OBJECT WILL BE DESTROYED and a new one will be created.
		/// Because of this the texture has to be reattached to all framebuffers after it got resized.
		/// </summary>
		/// <param name="width">of texture</param>
		/// <param name="height">of texture</param>
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
