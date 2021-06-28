#pragma once
#include "modelstructure/Mesh.h"
#include "modelstructure/CubeData.h"
#include "camera/Camera.h"

namespace glib {
	
	class SkyBox {

	public:
		SkyBox(std::int32_t textureUnit = 12);

		void setTextureUnit(std::int32_t unit);
		void draw(const Camera& cam, ShaderProgram& skyBoxProgram);

	private:
		Mesh mesh;
		std::int32_t textureUnit;

		void setUniforms(const Camera& cam, ShaderProgram& program);

	};

}