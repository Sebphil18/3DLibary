#include "lighting/SkyBoxLightProbe.h"
#include "glad/glad.h"
#include "modelstructure/CubeData.h"

namespace glib {
	
	SkyBoxLightProbe::SkyBoxLightProbe(CubeMapArray& skybox, const std::shared_ptr<ShaderProgram>& convProgram) :
		envMap({ 64, 64 }), convolutionProgram(convProgram)
	{
		bake(skybox);
	}

	void SkyBoxLightProbe::bake(CubeMapArray& skybox) {

		std::int32_t viewportSettings[4];
		glGetIntegerv(GL_VIEWPORT, viewportSettings);

		adjustViewport({ 64, 64 });

		setUniforms(skybox);
		convoluteSkybox();

		restoreViewport(viewportSettings);
	}

	void SkyBoxLightProbe::adjustViewport(glm::vec2 dimensions) {
		glViewport(0, 0, dimensions.x, dimensions.y);
	}

	void SkyBoxLightProbe::setUniforms(CubeMapArray& skybox) {

		skybox.bindToUnit(10);

		convolutionProgram->setUniformMat("projection", CubeMapArray::projection);
		convolutionProgram->setUniform("envMap", 10);
	}

	void SkyBoxLightProbe::convoluteSkybox() {

		Mesh cube(cubeVertices, cubeIndices);
		Framebuffer fbo;
		RenderBuffer rbo(64, 64, TextureType::DepthStencilAttachment);

		renderToCube(cube, fbo);
	}

	void SkyBoxLightProbe::renderToCube(Mesh& cube, Framebuffer& fbo) {

		fbo.bind();
		for (std::uint32_t i = 0; i < 6; i++) {

			convolutionProgram->setUniformMat("view", CubeMapArray::view[i]);

			fbo.attachTextureLayer(envMap.getGlHandle(), 0, i);
			fbo.clear();

			cube.draw(*convolutionProgram);
		}
		fbo.unbind();
	}

	void SkyBoxLightProbe::restoreViewport(std::int32_t settings[4]) {

		glViewport(
			settings[0],
			settings[1],
			settings[2],
			settings[3]);
	}

	void SkyBoxLightProbe::bindToUnit(std::uint32_t unit) {
		envMap.bindToUnit(unit);
	}

	std::uint32_t SkyBoxLightProbe::getGlHandle() const noexcept {
		return envMap.getGlHandle();
	}

}