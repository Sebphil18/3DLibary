#pragma once
#include "modelstructure/Mesh.h"

namespace otg {

	class ScreenMesh : public Mesh {

	public:
		static std::vector<otg::Vertex> screenVertices;
		static std::vector<unsigned int> screenIndices;
		
		ScreenMesh();

		void addTexture(const std::shared_ptr<Texture>& texture);

	};

}