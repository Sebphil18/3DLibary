#pragma once
#include <array>
#include "globjects/Texture.h"
#include "globjects/CubeMapArray.h"
#include "globjects/ShaderProgram.h"

namespace otg {

	class SkyBoxReflectionProbe {

	public:
		SkyBoxReflectionProbe(CubeMapArray& envMap, const std::shared_ptr<ShaderProgram>& prefilterProgram, const std::shared_ptr<ShaderProgram>& brdfIntegrationProgram);

		void bindPrefilterToUnit(std::uint32_t unit);
		void bindLookUpTexToUnit(std::uint32_t unit);

		void bake(CubeMapArray& envMap);

	private:
		CubeMapArray prefilterMap;
		Texture brdfIntegrationMap;
		Framebuffer fbo;

		std::shared_ptr<ShaderProgram> prefilterProgram, brdfIntegrationProgram;

		void prefilter();
		void renderMipMapLevels(Mesh& cube);
		glm::vec2 getMipMapDimensions(std::uint32_t mipmapLevel, glm::vec2 prefilterDimensions);
		void setRoughness(std::uint32_t mipmapLevel, std::uint32_t mipmapLevels);
		void renderCubeToCubeFaces(Mesh& cube, std::uint32_t mipmapLevel);

		void generateBrdfMap();
		void setUpLookUpTexture();

		void adjustViewport(glm::vec2 dimensions);

		void renderToTexture();

		void restoreViewport(std::int32_t settings[4]);

	};

}
