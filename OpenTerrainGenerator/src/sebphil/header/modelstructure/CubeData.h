#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "modelstructure/Vertex.h"

static std::vector<otg::Vertex> unitCubeVertices = {
	// front
	{ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(0.5, -0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(0.5, 0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	//right
	{ glm::vec3(0.5, -0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(0.5, -0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(0.5, 0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(0.5, 0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	//left
	{ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	//back
	{ glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(0.5, -0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(0.5, 0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	// top
	{ glm::vec3(-0.5, 0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(0.5, 0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(0.5, 0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	// bottom
	{ glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(0.5, -0.5, -0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(0.5, -0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-0.5, -0.5, 0.5), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) }
};

static std::vector<otg::Vertex> cubeVertices = {

	// front
	{ glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, 1, -1), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, 1, -1), glm::vec3(0, 0, -1), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	//right
	{ glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, -1, 1), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(1, 1, -1), glm::vec3(1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	//left
	{ glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(-1, -1, 1), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(-1, 1, 1), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, 1, -1), glm::vec3(-1, 0, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	//back
	{ glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, -1, 1), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	// top
	{ glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, 1, 1), glm::vec3(0, 1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) },

	// bottom
	{ glm::vec3(-1, -0.5, -1), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, -0.5, -1), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, -0.5, 1), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, -0.5, 1), glm::vec3(0, -1, 0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1)}

};

static std::vector<unsigned int> cubeIndices = {
	0, 1, 2, 0, 2, 3,
	4, 5, 6, 4, 6, 7,
	8, 9, 10, 8, 10, 11,
	12, 13, 14, 12, 14, 15,
	16, 17, 18, 16, 18, 19,
	20, 21, 22, 20, 22, 23
};
