#pragma once
#include "modelstructure/Mesh.h"

namespace glib {

	class ScreenMesh : public Mesh {

	public:
		static std::vector<glib::Vertex> screenVertices;
		static std::vector<unsigned int> screenIndices;
		
		ScreenMesh();

		void addTexture(const std::shared_ptr<Texture>& texture);

	};

}