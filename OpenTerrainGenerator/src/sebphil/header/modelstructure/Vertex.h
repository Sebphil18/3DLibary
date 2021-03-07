#pragma once
#include "glm/glm.hpp"
#include "globjects/VertexBufferLayout.h"

namespace otg {

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec3 bitangent;
		glm::vec2 texCoord;
	};

}