#include "modelstructure/Mesh.h"
#include "glad/glad.h"

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

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<std::shared_ptr<Texture>>& textures) noexcept :
		Mesh()
	{
		data.vertices = vertices;
		data.indices = indices;
		data.textures = textures;

		fillBuffer();
	}

	Mesh::Mesh(const MeshData& data) noexcept :
		data(data) {

		setUpLayout();
		fillBuffer();
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

	void Mesh::fillBuffer() {

		setVertexBufferData();
		setIndexBufferData();
	}

	void Mesh::setVertexBufferData() {

		if (!data.vertices.empty()) {

			std::size_t verticesCount = data.vertices.size();
			vbo.setData(verticesCount * sizeof(Vertex), &data.vertices[0], GL_STATIC_DRAW);
		}
	}

	void Mesh::setIndexBufferData() {

		if (!data.indices.empty()) {

			std::size_t indicesCount = data.indices.size();
			ibo.setData(indicesCount * sizeof(int), &data.indices[0], GL_STATIC_DRAW);
		}
	}

	void Mesh::updateVertices(const std::vector<Vertex>& vertices, std::size_t offset, std::size_t count) {

		// TODO: implement more efficient method
		this->data.vertices = vertices;

		updateVertexBuffer(offset, count);
	}

	void Mesh::updateVertexBuffer(std::size_t offset, std::size_t count) {

		std::size_t size = count * sizeof(Vertex);

		vbo.updateData(static_cast<std::uint32_t>(size), static_cast<std::uint32_t>(offset), &data.vertices[offset]);
	}

	void Mesh::draw(otg::ShaderProgram& program) {

		program.use();
		vao.use();

		bindTextures(program);
		bindMaterial(program);
		drawTriangles();
		unbindTextures(program);
	}

	void Mesh::bindTextures(otg::ShaderProgram& program) {

		TextureIterators iterators;

		for (std::int32_t unit = 1; unit <= static_cast<int>(data.textures.size()); unit++) {
			
			const std::shared_ptr<Texture>& texture = data.textures[unit - 1];

			texture->bindToUnit(unit);

			std::string uniName = getTexUniName(iterators, texture->getType());
			program.setUniform(uniName, unit);
		}
	}

	void Mesh::bindMaterial(otg::ShaderProgram& program) {

		program.setUniform("material.roughness", data.material.roughness);
		program.setUniform("material.metallic", data.material.metallic);
		program.setUniform("material.occlusion", data.material.occlusion);
		program.setUniformVec("material.albedoColor", data.material.albedoColor);
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

	// TODO: inefficient
	void Mesh::unbindTextures(otg::ShaderProgram& program) {

		TextureIterators iterators;

		for (std::int32_t unit = 1; unit <= static_cast<int>(data.textures.size()); unit++) {

			const std::shared_ptr<Texture>& texture = data.textures[unit - 1];

			std::string uniName = getTexUniName(iterators, texture->getType());
			program.setUniform(uniName, 0);
		}
	}

	std::string Mesh::getTexUniName(TextureIterators& iterators, otg::TextureType type) {

		std::string uniName = "";

		switch (type) {

		case TextureType::Albedo: setTexUniName(iterators.albedoTextures, "material.albedoTex", uniName);
			break;
		case TextureType::Roughness: setTexUniName(iterators.roughnessTextures, "material.roughnessTex", uniName);
			break;
		case TextureType::Metallic: setTexUniName(iterators.metallicTextures, "material.metallicTex", uniName);
			break;
		case TextureType::Occlusion: setTexUniName(iterators.occlussionTextures, "material.occlusionTex", uniName);
			break;
		case TextureType::Displacement: setTexUniName(iterators.displacementTextures, "material.displacement", uniName);
			break;
		default:
			setTexUniName(iterators.albedoTextures, "material.albedoTex", uniName);
		}

		return uniName;
	}

	void Mesh::setTexUniName(std::uint32_t& iterator, const std::string& uniPrefix, std::string& uniName) {
		uniName = uniPrefix + std::to_string(iterator);
		iterator++;
	}

	std::uint32_t Mesh::getVaoHandle() const {
		return vao.getGlHandle();
	}

	void Mesh::addTexture(const std::shared_ptr<Texture>& texture) {
		data.textures.push_back(texture);
	}

	void Mesh::removeTexture(std::size_t index) {
		data.textures.erase(data.textures.begin() + index);
	}

	void Mesh::clearTextures() {
		data.textures.clear();
	}

	void Mesh::setMaterial(const Material& material) {
		data.material = material;
	}

}