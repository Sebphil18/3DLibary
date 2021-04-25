#include "lighting/HDRTextureCube.h"
#include "modelstructure/CubeData.h"
#include "glm/gtx/transform.hpp"
#include "glad/glad.h"

namespace otg {

	glm::mat4 HDRTextureCube::projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 HDRTextureCube::view[6] = {
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
		glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)),
	};

	HDRTextureCube::HDRTextureCube(const std::shared_ptr<HDRTexture>& texture) :
		texture(texture), cubeMesh(cubeVertices, cubeIndices)
	{
	}

	void HDRTextureCube::draw(ShaderProgram& program, std::uint32_t face) {

		texture->bindToUnit(0);

		program.setUniform("equirectangularMap", 0);
		program.setUniformMat("projection", projection);
		program.setUniformMat("view", view[face]);

		cubeMesh.draw(program);
	}

}