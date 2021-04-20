#include "lighting/SkyBoxLightProbe.h"
#include "glad/glad.h"
#include "modelstructure/CubeData.h"

namespace otg {
	
	SkyBoxLightProbe::SkyBoxLightProbe(CubeMapArray& skybox, ShaderProgram& convProgram) : 
		envMap(64, 64)
	{
		std::int32_t viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glViewport(0, 0, 64, 64);

		setUniforms(skybox, convProgram);
		convoluteSkybox(convProgram);

		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	}

	void SkyBoxLightProbe::setUniforms(CubeMapArray& skybox, ShaderProgram& convProgram) {

		skybox.bindToUnit(0);

		convProgram.use();
		convProgram.setUniformMat("projection", CubeMapArray::projection);
		convProgram.setUniform("envMap", 0);
	}

	void SkyBoxLightProbe::convoluteSkybox(ShaderProgram& convProgram) {

		Mesh cube(cubeVertices, cubeIndices);
		Framebuffer fbo;
		RenderBuffer rbo(64, 64, TextureType::DepthStencilAttachment);

		renderToCube(cube, fbo, convProgram);
	}

	void SkyBoxLightProbe::renderToCube(Mesh& cube, Framebuffer& fbo, ShaderProgram& convProgram) {

		fbo.bind();
		for (std::uint32_t i = 0; i < 6; i++) {

			convProgram.setUniformMat("view", CubeMapArray::view[i]);

			fbo.attachTextureLayer(envMap.getGlHandle(), 0, i);
			fbo.clear();

			cube.draw(convProgram);
		}
		fbo.unbind();
	}

	void SkyBoxLightProbe::bindToUnit(std::uint32_t unit) {
		envMap.bindToUnit(unit);
	}

	std::uint32_t SkyBoxLightProbe::getGlHandle() const noexcept {
		return envMap.getGlHandle();
	}

}