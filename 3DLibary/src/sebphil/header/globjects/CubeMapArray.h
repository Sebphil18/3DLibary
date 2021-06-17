#pragma once
#include <memory>
#include "io/Image.h"
#include "globjects/GlObject.h"
#include "globjects/TextureType.h"
#include "globjects/HDRTexture.h"
#include "globjects/ShaderProgram.h"
#include "globjects/Framebuffer.h"
#include "lighting/HDRTextureCube.h"

namespace otg {

	class CubeMapArray : public GlObject, public TextureTypes{

	public:
		CubeMapArray(const ImageFormat& imgFormat, std::uint32_t count = 1) noexcept;
		CubeMapArray(const ImageFormat& imgFormat, std::uint32_t levels, std::uint32_t count = 1) noexcept;
		CubeMapArray(const CubeMapArray& other) noexcept;
		CubeMapArray(CubeMapArray&& other) noexcept;

		CubeMapArray& operator=(const CubeMapArray& other) noexcept;
		CubeMapArray& operator=(CubeMapArray&& other) noexcept;

		~CubeMapArray() noexcept;

		void bindToUnit(std::uint32_t unit);

		void fromEquirectengular(const std::shared_ptr<HDRTexture>& equiRectTexture, ShaderProgram& conversionProgram);

		std::int32_t getWidth() const;
		std::int32_t getHeight() const;

		static const glm::mat4 projection;
		static const glm::mat4 view[6];

	private:
		ImageFloat img;
		std::uint32_t count, levels;

		void init();
		void createTexture();
		void setParameters();

		void enableMipmaps();

		void deleteTexture();

		void convertEquirectengular(const std::shared_ptr<HDRTexture>& equiRectTexture, ShaderProgram& conversionProgram);
		void renderToCube(HDRTextureCube& hdrCube, Framebuffer& fbo, ShaderProgram& conversionProgram);

	};

}