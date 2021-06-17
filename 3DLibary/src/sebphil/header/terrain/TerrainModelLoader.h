#pragma once
#include <mutex>
#include <future>
#include "terrain/Heightmap.h"
#include "modelstructure/Model.h"

namespace otg {

	class TerrainModelLoader {

	public:
		TerrainModelLoader(const Heightmap<float>& heightmap);

		void update(const Heightmap<float>& heightmap);

		bool isLoaded() const;
		ModelData getData() const;

	private:

		struct Triangle {
			Vertex* v1, * v2, * v3;
			glm::vec3 edge1, edge2;
			glm::vec2 uvEdge1, uvEdge2;
		};

		glm::ivec2 dimensions;
		std::mutex workerLock;
		std::future<void> worker;

		DeferredModelData data;

		void load(const Heightmap<float>& heightmap);

		std::vector<Vertex> getVertices(const Heightmap<float>& heightmap);
		Vertex getVertex(glm::vec2 position, const Heightmap<float>& heightmap);

		std::vector<std::uint32_t> getFaces(const Heightmap<float>& heightmap);
		void addFaces(std::uint32_t i, std::uint32_t width, std::vector<std::uint32_t>& indices);

		// TODO: input variables
		void generateTBN(DeferredMeshData& meshData, const Heightmap<float>& heightmap);
		void normalizeTBN(std::vector<Vertex>& vertices);

		void processFace(std::uint32_t index, std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices);
		Triangle constructTriangle(Vertex& v1, Vertex& v2, Vertex& v3);
		void generateNormal(Triangle& triangle);
		void generateTangent(Triangle& triangle);

	};

}