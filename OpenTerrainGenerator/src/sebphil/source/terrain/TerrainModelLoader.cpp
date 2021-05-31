#include "terrain/TerrainModelLoader.h"
#include "modelstructure/ModelDataConverter.h"

namespace otg {

	TerrainModelLoader::TerrainModelLoader(const Heightmap<float>& heightmap) : 
		dimensions(heightmap.getSize())
	{

		worker = std::async(std::launch::async, &TerrainModelLoader::load, this, heightmap);
	}

	void TerrainModelLoader::load(const Heightmap<float>& heightmap) {

		std::scoped_lock<std::mutex> lock(workerLock);

		DeferredMeshData meshData;

		meshData.vertices = getVertices(heightmap);
		meshData.indices = getFaces(heightmap);
		generateTBN(meshData, heightmap);

		data.meshes.push_back(meshData);
	}

	std::vector<Vertex> TerrainModelLoader::getVertices(const Heightmap<float>& heightmap) {

		std::vector<Vertex> vertices;
		vertices.reserve(static_cast<std::size_t>(dimensions.x) * dimensions.y);

		for (std::int32_t x = 0; x < dimensions.x; x++) {
			for (std::int32_t z = 0; z < dimensions.y; z++) {

				Vertex vertex = getVertex({ x, z }, heightmap);
				vertices.push_back(vertex);
			}
		}

		return vertices;
	}

	Vertex TerrainModelLoader::getVertex(glm::vec2 position, const Heightmap<float>& heightmap) {

		float y = heightmap.getValue(position);

		Vertex vertex;
		vertex.position = { position.x, y, position.y };
		vertex.texCoord = position * 0.2f;

		return vertex;
	}

	std::vector<std::uint32_t> TerrainModelLoader::getFaces(const Heightmap<float>& heightmap) {

		std::vector<std::uint32_t> indices;
		indices.reserve(static_cast<std::size_t>(dimensions.x) * dimensions.y);

		for (std::int32_t x = 0; x < dimensions.x - 1; x++) {
			for (std::int32_t z = 0; z < dimensions.y - 1; z++) {

				std::uint32_t i = heightmap.getIndex({ x, z });;
				addFaces(i, dimensions.x, indices);
			}
		}

		return indices;
	}

	void TerrainModelLoader::addFaces(std::uint32_t i, std::uint32_t width, std::vector<std::uint32_t>& indices) {

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + width + 1);

		indices.push_back(i);
		indices.push_back(i + width + 1);
		indices.push_back(i + width);
	}

	void TerrainModelLoader::generateTBN(DeferredMeshData& meshData, const Heightmap<float>& heightmap) {

		std::vector<Vertex>& vertices = meshData.vertices;
		std::vector<std::uint32_t>& indices = meshData.indices;

		std::uint32_t faces = static_cast<std::uint32_t>(meshData.indices.size()) / 3;

		for (std::uint32_t face = 0; face < faces; face++) {

			std::uint32_t index = face * 3;
			processFace(index, vertices, indices);
		}

		normalizeTBN(vertices);
	}

	void TerrainModelLoader::processFace(std::uint32_t index, std::vector<Vertex>& vertices, std::vector<std::uint32_t>& indices) {

		Vertex& v1 = vertices[indices[index]];
		Vertex& v2 = vertices[indices[index + 1]];
		Vertex& v3 = vertices[indices[index + 2]];

		Triangle tri = constructTriangle(v1, v2, v3);
		generateNormal(tri);
		generateTangent(tri);
	}

	TerrainModelLoader::Triangle TerrainModelLoader::constructTriangle(Vertex& v1, Vertex& v2, Vertex& v3) {

		Triangle triangle = { &v1, &v2, &v3 };

		triangle.edge1 = v2.position - v1.position;
		triangle.edge2 = v3.position - v1.position;

		triangle.uvEdge1 = v2.texCoord - v1.texCoord;
		triangle.uvEdge2 = v3.texCoord - v1.texCoord;

		return triangle;
	}

	void TerrainModelLoader::generateNormal(Triangle& triangle) {

		glm::vec3 normal = glm::cross(triangle.edge1, triangle.edge2);

		triangle.v1->normal += normal;
		triangle.v2->normal += normal;
		triangle.v3->normal += normal;
	}

	void TerrainModelLoader::generateTangent(Triangle& triangle) {

		float a = 1 / (triangle.uvEdge1.x * triangle.uvEdge2.y - triangle.uvEdge1.y * triangle.uvEdge2.x);
		glm::vec3 tangent = a * (triangle.edge1 * triangle.uvEdge2.y - triangle.edge2 * triangle.uvEdge1.y);
		glm::vec3 bitangent = a * (triangle.edge2 * triangle.uvEdge1.x - triangle.edge1 * triangle.uvEdge2.x);

		triangle.v1->tangent += tangent;
		triangle.v2->tangent += tangent;
		triangle.v3->tangent += tangent;

		triangle.v1->bitangent += bitangent;
		triangle.v2->bitangent += bitangent;
		triangle.v3->bitangent += bitangent;
	}

	void TerrainModelLoader::normalizeTBN(std::vector<Vertex>& vertices) {

		for (Vertex& vertex : vertices) {

			vertex.normal = glm::normalize(vertex.normal);
			vertex.tangent = glm::normalize(vertex.tangent);
			vertex.bitangent = glm::normalize(vertex.bitangent);
		}
	}

	bool TerrainModelLoader::isLoaded() const {

		std::future_status status = worker.wait_for(std::chrono::seconds(0));
		return status == std::future_status::ready;
	}

	/// <summary>
	/// This function returns the ModelData of the generated terrain and blocks until the mesh was generated.
	/// </summary>
	/// <returns>ModelData of generated model</returns>
	ModelData TerrainModelLoader::getData() const {

		worker.wait();
		return ModelDataConverter::convert(data);
	}
}