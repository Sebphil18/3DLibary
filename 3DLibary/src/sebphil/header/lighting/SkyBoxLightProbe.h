#pragma once
#include <memory>
#include "glm/glm.hpp"
#include "globjects/CubeMapArray.h"
#include "globjects/ShaderProgram.h"

namespace otg {

	class SkyBoxLightProbe {

	public:
		SkyBoxLightProbe(CubeMapArray& skybox, const std::shared_ptr<ShaderProgram>& convProgram);

		void bindToUnit(std::uint32_t unit);

		void bake(CubeMapArray& skybox);

		std::uint32_t getGlHandle() const noexcept;

	private:
		CubeMapArray envMap;
		std::shared_ptr<ShaderProgram> convolutionProgram;

		void adjustViewport(glm::vec2 dimensions);

		void setUniforms(CubeMapArray& skybox);
		void convoluteSkybox();
		void renderToCube(Mesh& cube, Framebuffer& fbo);

		void restoreViewport(std::int32_t settings[4]);
	};

}