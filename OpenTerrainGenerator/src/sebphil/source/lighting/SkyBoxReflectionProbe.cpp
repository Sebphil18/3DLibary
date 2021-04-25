#include "lighting/SkyBoxReflectionProbe.h"
#include "glad/glad.h"
#include "modelstructure/CubeData.h"
#include "modelstructure/ScreenMesh.h"

namespace otg {

	SkyBoxReflectionProbe::SkyBoxReflectionProbe(CubeMapArray& envMap,
		const std::shared_ptr<ShaderProgram>& prefilterProgram, const std::shared_ptr<ShaderProgram>& brdfIntegrationProgram) :

		prefilterMap({ 256, 256 }, 5, 1),
		brdfIntegrationMap(512, 512, TextureType::RGBFloat),
		prefilterProgram(prefilterProgram),
		brdfIntegrationProgram(brdfIntegrationProgram) 	{

		bake(envMap);
	}

	void SkyBoxReflectionProbe::bake(CubeMapArray& envMap) {

		envMap.bindToUnit(0);

		std::int32_t viewportSettings[4];
		glGetIntegerv(GL_VIEWPORT, viewportSettings);

		prefilter();
		generateBrdfMap();

		restoreViewport(viewportSettings);
	}

	void SkyBoxReflectionProbe::prefilter() {

		prefilterProgram->setUniformMat("projection", CubeMapArray::projection);
		prefilterProgram->setUniform("envMap", 0);

		Mesh cube(cubeVertices, cubeIndices);

		fbo.bind();
		renderMipMapLevels(cube);
		fbo.unbind();
	}

	void SkyBoxReflectionProbe::renderMipMapLevels(Mesh& cube) {

		glm::vec2 prefilterDimensions = glm::vec2(prefilterMap.getWidth(), prefilterMap.getHeight());
		std::uint32_t mipmapLevels = 5;

		for (std::uint32_t mipmapLevel = 0; mipmapLevel < mipmapLevels; mipmapLevel++) {

			glm::vec2 mipmapDimensions = getMipMapDimensions(mipmapLevel, prefilterDimensions);

			setRoughness(mipmapLevel, mipmapLevels);

			adjustViewport(mipmapDimensions);
			renderCubeToCubeFaces(cube, mipmapLevel);
		}
	}

	glm::vec2 SkyBoxReflectionProbe::getMipMapDimensions(std::uint32_t mipmapLevel, glm::vec2 prefilterDimensions) {

		return glm::vec2(
			prefilterDimensions.x * std::pow(0.5, mipmapLevel),
			prefilterDimensions.y * std::pow(0.5, mipmapLevel));
	}

	void SkyBoxReflectionProbe::setRoughness(std::uint32_t mipmapLevel, std::uint32_t mipmapLevels) {

		float roughness = mipmapLevel / static_cast<float>(mipmapLevels - 1);
		prefilterProgram->setUniform("roughness", roughness);
	}

	void SkyBoxReflectionProbe::renderCubeToCubeFaces(Mesh& cube, std::uint32_t mipmapLevel) {

		for (std::uint32_t face = 0; face < 6; face++) {

			prefilterProgram->setUniformMat("view", CubeMapArray::view[face]);
			fbo.attachTextureLayer(prefilterMap.getGlHandle(), mipmapLevel, face);
			fbo.clear();

			cube.draw(*prefilterProgram);
		}
	}

	void SkyBoxReflectionProbe::generateBrdfMap() {

		setUpLookUpTexture();

		glm::vec2 textureDimensions = glm::vec2(brdfIntegrationMap.getWidth(), brdfIntegrationMap.getHeight());

		adjustViewport(textureDimensions);
		renderToTexture();
	}

	void SkyBoxReflectionProbe::setUpLookUpTexture() {

		glTextureParameteri(brdfIntegrationMap.getGlHandle(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(brdfIntegrationMap.getGlHandle(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void SkyBoxReflectionProbe::adjustViewport(glm::vec2 dimensions) {

		glViewport(0, 0, dimensions.x, dimensions.y);
	}

	void SkyBoxReflectionProbe::renderToTexture() {

		fbo.bind();
		fbo.attachTexture(brdfIntegrationMap);
		fbo.clear();

		ScreenMesh quad;
		quad.draw(*brdfIntegrationProgram);

		fbo.unbind();
	}

	void SkyBoxReflectionProbe::restoreViewport(std::int32_t settings[4]) {
		glViewport(
			settings[0],
			settings[1],
			settings[2],
			settings[3]);
	}

	void SkyBoxReflectionProbe::bindPrefilterToUnit(std::uint32_t unit) {
		prefilterMap.bindToUnit(unit);
	}

	void SkyBoxReflectionProbe::bindLookUpTexToUnit(std::uint32_t unit) {
		brdfIntegrationMap.bindToUnit(unit);
	}

}