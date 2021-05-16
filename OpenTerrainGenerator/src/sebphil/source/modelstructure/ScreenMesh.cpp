#include "modelstructure/ScreenMesh.h"

namespace otg {

	// standard VertexFormat: position | normal | tangent | bitangent | textureCoordinates
	std::vector<otg::Vertex> ScreenMesh::screenVertices = {
	{ glm::vec3(-1, -1, -1), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 0) },
	{ glm::vec3(1, -1, -1), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 0) },
	{ glm::vec3(1, 1, -1), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(1, 1) },
	{ glm::vec3(-1, 1, -1), glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec2(0, 1) }
	};

	std::vector<unsigned int> ScreenMesh::screenIndices = {
			0, 1, 3,
			1, 2, 3,
	};

	ScreenMesh::ScreenMesh() : 
		Mesh(screenVertices, screenIndices)
	{
	}

	void ScreenMesh::addTexture(const std::shared_ptr<Texture>& texture) {
		data.material.addTexture(texture);
	}

}