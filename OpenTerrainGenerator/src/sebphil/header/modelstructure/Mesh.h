#pragma once
#include <vector>
#include <memory>
#include "io/Image.h"
#include "globjects/VertexBuffer.h"
#include "globjects/IndexBuffer.h"
#include "globjects/VertexArray.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/ShaderProgram.h"
#include "globjects/Texture.h"
#include "globjects/TextureType.h"
#include "modelstructure/Vertex.h"
#include "glm/glm.hpp"

namespace otg {

	struct Material {
		float roughness = 0;
		float metallic = 0;
		float occlusion = 1;
		glm::vec3 albedoColor = glm::vec3(1, 0, 1);
		glm::vec3 specularColor = glm::vec3(1, 1, 0);
		glm::vec3 ambientColor = glm::vec3(0, 1, 1);
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;
		std::vector<std::shared_ptr<Texture>> textures;
		Material material;
	};

	struct DeferredTexture {
		std::string filePath;
		TextureType type;
	};

	struct DeferredMeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;
		std::vector<DeferredTexture> texturePaths;
		Material material;
	};

	class Mesh {

		struct TextureIterators {
			std::uint32_t albedoTextures = 0;
			std::uint32_t roughnessTextures = 0;
			std::uint32_t occlussionTextures = 0;
			std::uint32_t metallicTextures = 0;
			std::uint32_t displacementTextures = 0;
		};

	public:
		Mesh() noexcept;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures) noexcept;
		Mesh(const Mesh& mesh) noexcept;
		Mesh(const MeshData& data) noexcept;
		Mesh(Mesh&& other) = default;

		Mesh& operator=(const Mesh& mesh) noexcept;

		void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void setData(const MeshData& data);
		void updateVertices(const std::vector<Vertex>& vertices, std::size_t offset, std::size_t count);

		void draw(otg::ShaderProgram& program);

		void addTexture(const std::shared_ptr<Texture>& texture);
		void removeTexture(std::size_t index);
		void clearTextures();

		void setMaterial(const Material& material);

		std::uint32_t getVaoHandle() const;

	private:
		MeshData data;

		VertexBuffer vbo;
		IndexBuffer ibo;
		VertexArray vao;
		VertexBufferLayout layout;
		
		void setUpLayout();
		void setLayoutElements();
		void applyLayout(const VertexBufferLayout& layout);

		void fillBuffer();
		void setVertexBufferData();
		void setIndexBufferData();

		void updateVertexBuffer(std::size_t offset, std::size_t count);

		void bindTextures(otg::ShaderProgram& program);
		std::string getTexUniName(TextureIterators& iterators, TextureType type);
		void setTexUniName(std::uint32_t& iterator, const std::string& uniPrefix, std::string& uniName);

		void bindMaterial(otg::ShaderProgram& program);

		void drawTriangles();
		void drawIndices();
		void drawVertices();

		void unbindTextures(otg::ShaderProgram& program);

	};

}