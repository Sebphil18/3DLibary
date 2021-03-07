#pragma once
#include <vector>
#include <memory>
#include "io/Image.h"
#include "globjects/VertexBuffer.h"
#include "globjects/IndexBuffer.h"
#include "globjects/VertexArray.h"
#include "globjects/VertexBufferLayout.h"
#include "globjects/ShaderProgram.h"
#include "globjects/TextureImage.h"
#include "modelstructure/TextureType.h"
#include "modelstructure/Vertex.h"

namespace otg {

	// TODO: implementation comes with shaders
	struct Material {
		float roughness;
		float metalness;
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;
		std::vector<std::shared_ptr<TextureImage>> textures;
		Material material = {0, 0};
	};

	class Mesh {

		struct TextureIterators {
			std::uint32_t albedoTextures = 0;
			std::uint32_t roughnessTextures = 0;
			std::uint32_t occlussionTextures = 0;
			std::uint32_t metalnessTextures = 0;
			std::uint32_t heightTextures = 0;
		};

	public:
		Mesh();
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<TextureImage>>& textures);
		Mesh(const MeshData& data);

		void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void updateVertices(const std::vector<Vertex>& vertices, std::size_t offset, std::size_t count);

		void draw(otg::ShaderProgram& program);

		void addTexture(const std::shared_ptr<TextureImage>& texture);

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
		void bindTexToUnit(const std::shared_ptr<TextureImage>& texture, int unit);
		std::string getTexUniName(TextureIterators& iterators, TextureType type);

		void drawTriangles();
		void drawIndices();
		void drawVertices();

	};

}