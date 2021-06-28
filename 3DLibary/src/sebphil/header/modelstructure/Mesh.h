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
#include "material/Material.h"
#include "glm/glm.hpp"
#include "glad/glad.h"

namespace glib {

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;
		Material material;
	};

	struct DeferredMeshData {
		std::vector<Vertex> vertices;
		std::vector<std::uint32_t> indices;
		DeferredMaterial material;
	};

	class Mesh {

	public:
		Mesh() noexcept;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material) noexcept;
		Mesh(const Mesh& mesh) noexcept;
		Mesh(const MeshData& data) noexcept;
		Mesh(const MeshData& data, std::uint32_t usage) noexcept;
		Mesh(Mesh&& other) = default;

		Mesh& operator=(const Mesh& mesh) noexcept;

		void setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void setData(const MeshData& data);
		void updateVertices(const std::vector<Vertex>& vertices, std::size_t offset, std::size_t count);

		void draw(glib::ShaderProgram& program);

		void setMaterial(const Material& material);

		std::uint32_t getVaoHandle() const;

	protected:
		MeshData data;

	private:
		VertexBuffer vbo;
		IndexBuffer ibo;
		VertexArray vao;
		VertexBufferLayout layout;
		
		void setUpLayout();
		void setLayoutElements();
		void applyLayout(const VertexBufferLayout& layout);

		void fillBuffer(std::uint32_t usage = GL_STATIC_DRAW);
		void setVertexBufferData(std::uint32_t usage);
		void setIndexBufferData();

		void updateVertexBuffer(std::size_t offset, std::size_t count);

		void drawTriangles();
		void drawIndices();
		void drawVertices();

		void unbindTextures(glib::ShaderProgram& program);

	};

}