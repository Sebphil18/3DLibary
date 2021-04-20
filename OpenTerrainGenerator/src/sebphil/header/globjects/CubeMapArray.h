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
		CubeMapArray(std::int32_t width, std::int32_t height, std::uint32_t count = 1) noexcept;
		CubeMapArray(const CubeMapArray& other) noexcept;
		CubeMapArray(CubeMapArray&& other) noexcept;

		CubeMapArray& operator=(const CubeMapArray& other) noexcept;
		CubeMapArray& operator=(CubeMapArray&& other) noexcept;

		~CubeMapArray() noexcept;

		void bindToUnit(std::uint32_t unit);

		void fromEquirectengular(const std::shared_ptr<HDRTexture>& equiRectTexture, ShaderProgram& conversionProgram);

		static const glm::mat4 projection;
		static const glm::mat4 view[6];

	private:
		Image img;
		std::uint32_t count;

		void init();
		void createTexture();
		void setParameters();

		void deleteTexture();

		void convertEquirectengular(const std::shared_ptr<HDRTexture>& equiRectTexture, ShaderProgram& conversionProgram);
		void renderToCube(HDRTextureCube& hdrCube, Framebuffer& fbo, ShaderProgram& conversionProgram);

	};

}