#pragma once
#include "glm/glm.hpp"
#include "globjects/VertexBufferLayout.h"

namespace glib {

	struct Vertex {
		glm::vec3 position = glm::vec3(0);
		glm::vec3 normal = glm::vec3(0);
		glm::vec3 tangent = glm::vec3(0);
		glm::vec3 bitangent = glm::vec3(0);
		glm::vec2 texCoord = glm::vec2(0);
	};

}