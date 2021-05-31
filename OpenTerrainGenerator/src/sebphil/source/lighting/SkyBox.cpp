#include "lighting/SkyBox.h"

namespace otg {

	SkyBox::SkyBox(std::int32_t textureUnit) :
		mesh(cubeVertices, cubeIndices), textureUnit(textureUnit)
	{
	}

	void SkyBox::setTextureUnit(std::int32_t unit) {
		this->textureUnit = unit;
	}

	void SkyBox::draw(const Camera& cam, ShaderProgram& skyBoxProgram) {
		glDepthFunc(GL_LEQUAL);
		setUniforms(cam, skyBoxProgram);
		mesh.draw(skyBoxProgram);
		glDepthFunc(GL_LESS);
	}

	void SkyBox::setUniforms(const Camera& cam, ShaderProgram& program) {
		program.setUniform("cubeMap", textureUnit);
		program.setUniformMat("view", cam.getViewMatrix());
		program.setUniformMat("projection", cam.getProjectionMatrix());
	}

}