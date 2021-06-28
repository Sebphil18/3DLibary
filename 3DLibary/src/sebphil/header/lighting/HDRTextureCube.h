#pragma once
#include "globjects/HDRTexture.h"
#include "globjects/ShaderProgram.h"
#include "modelstructure/Mesh.h"

namespace glib {

	class HDRTextureCube {

	public:
		HDRTextureCube(const std::shared_ptr<HDRTexture>& texture);

		void draw(ShaderProgram& program, std::uint32_t face);

	private:
		Mesh cubeMesh;
		std::shared_ptr<HDRTexture> texture;

		static glm::mat4 projection;
		static glm::mat4 view[6];

	};

}