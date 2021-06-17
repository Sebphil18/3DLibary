#include "modelstructure/Mesh.h"
#include <algorithm>

namespace otg {

	Mesh::Mesh() noexcept {

		setUpLayout();
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) noexcept : 
		Mesh()
	{
		data.vertices = vertices;
		data.indices = indices;

		fillBuffer();
	}

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const Material& material) noexcept :
		Mesh()
	{
		data.vertices = vertices;
		data.indices = indices;
		data.material = material;

		fillBuffer();
	}

	Mesh::Mesh(const MeshData& data) noexcept :
		data(data) 
	{
		setUpLayout();
		fillBuffer();
	}

	Mesh::Mesh(const MeshData& data, std::uint32_t usage) noexcept :
		data(data)
	{
		setUpLayout();
		fillBuffer(usage);
	}

	Mesh::Mesh(const Mesh& otherMesh) noexcept :
		Mesh(otherMesh.data)
	{
	}

	Mesh& otg::Mesh::operator=(const Mesh& otherMesh) noexcept {
		data = otherMesh.data;

		setUpLayout();
		fillBuffer();

		return *this;
	}

	void Mesh::setUpLayout() {

		setLayoutElements();
		applyLayout(layout);
	}

	void Mesh::setLayoutElements() {

		layout.addElement({ ElementType::Float, 3 });
		layout.addElement({ ElementType::Float, 3 });
		layout.addElement({ ElementType::Float, 3 });
		layout.addElement({ ElementType::Float, 3 });
		layout.addElement({ ElementType::Float, 2 });
	}

	void Mesh::applyLayout(const VertexBufferLayout& layout) {
		layout.applyLayout(vao.getGlHandle(), vbo.getGlHandle(), ibo.getGlHandle());
	}

	void Mesh::setData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {

		this->data.vertices = vertices;
		this->data.indices = indices;

		fillBuffer();
	}

	void Mesh::setData(const MeshData& data) {

		this->data = data;

		fillBuffer();
	}

	void Mesh::fillBuffer(std::uint32_t usage) {

		setVertexBufferData(usage);
		setIndexBufferData();
	}

	void Mesh::setVertexBufferData(std::uint32_t usage) {

		if (!data.vertices.empty()) {

			std::size_t verticesCount = data.vertices.size();
			vbo.setData(verticesCount * sizeof(Vertex), &data.vertices[0], usage);
		}
	}

	void Mesh::setIndexBufferData() {

		if (!data.indices.empty()) {

			std::size_t indicesCount = data.indices.size();
			ibo.setData(indicesCount * sizeof(int), &data.indices[0], GL_STATIC_DRAW);
		}
	}

	void Mesh::updateVertices(const std::vector<Vertex>& vertices, std::size_t offset, std::size_t count) {

		auto begin = vertices.begin();
		std::size_t endOffset = offset + count;

		std::copy(begin + offset, begin + endOffset, data.vertices.begin() + offset);

		updateVertexBuffer(offset, count);
	}

	void Mesh::updateVertexBuffer(std::size_t offset, std::size_t count) {

		std::size_t size = count * sizeof(Vertex);

		vbo.updateData(static_cast<std::uint32_t>(size), static_cast<std::uint32_t>(offset), &data.vertices[offset]);
	}

	void Mesh::draw(otg::ShaderProgram& program) {

		program.use();
		vao.use();

		data.material.bindTexture(program);
		data.material.setUniforms(program);
		drawTriangles();
		data.material.unbindTextures(program);
	}

	void Mesh::drawTriangles() {

		if (!data.indices.empty())
			drawIndices();
		else
			drawVertices();
	}

	void Mesh::drawIndices() {
		std::size_t indexCount = data.indices.size();
		glDrawElements(GL_TRIANGLES, static_cast<std::uint32_t>(indexCount), GL_UNSIGNED_INT, 0);
	}

	void Mesh::drawVertices() {
		std::size_t vertexCount = data.vertices.size();
		glDrawArrays(GL_TRIANGLES, 0, static_cast<std::uint32_t>(data.vertices.size()));
	}

	void Mesh::setMaterial(const Material& material) {
		data.material = material;
	}

}