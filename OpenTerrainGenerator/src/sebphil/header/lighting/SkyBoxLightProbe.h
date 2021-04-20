#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "globjects/CubeMapArray.h"
#include "globjects/ShaderProgram.h"

namespace otg {

	class SkyBoxLightProbe {

	public:
		SkyBoxLightProbe(CubeMapArray& skybox, ShaderProgram& convProgram);

		void bindToUnit(std::uint32_t unit);

		std::uint32_t getGlHandle() const noexcept;

	private:
		CubeMapArray envMap;

		void setUniforms(CubeMapArray& skybox, ShaderProgram& convProgram);
		void convoluteSkybox(ShaderProgram& convProgram);
		void renderToCube(Mesh& cube, Framebuffer& fbo, ShaderProgram& convProgram);

	};

}