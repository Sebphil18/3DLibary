#include "globjects/VertexBuffer.h"
#include <iostream>
#include "glad/glad.h"

// TODO: buffer allocates data array of length 0 on construction
glib::VertexBuffer::VertexBuffer() noexcept :
	usage(GL_STATIC_DRAW), size(0), data(nullptr)
{
	createBuffer();
	fillBuffer();
}

glib::VertexBuffer::VertexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept :
	usage(usage), size(size), data(data)
{
	createBuffer();
	fillBuffer();
}

glib::VertexBuffer::VertexBuffer(const VertexBuffer& otherVbo) noexcept :
	VertexBuffer(otherVbo.size, otherVbo.data, otherVbo.usage)
{
}

glib::VertexBuffer& glib::VertexBuffer::operator=(const VertexBuffer& otherVbo) noexcept {

	usage = otherVbo.usage;
	size = otherVbo.size;
	data = otherVbo.data;

	fillBuffer();

	return *this;
}

void glib::VertexBuffer::createBuffer() {

	glCreateBuffers(1, &glHandle);
}

glib::VertexBuffer::VertexBuffer(VertexBuffer&& otherVbo) noexcept :
	GlObject(std::move(otherVbo)),
	usage(std::move(otherVbo.usage)),
	size(std::move(otherVbo.size)),
	data(std::move(otherVbo.data))
{
}

glib::VertexBuffer& glib::VertexBuffer::operator=(VertexBuffer&& otherVbo) noexcept {

	GlObject::operator=(std::move(otherVbo));

	usage = std::move(otherVbo.usage);
	size = std::move(otherVbo.size);
	data = std::move(otherVbo.data);

	return *this;
}

glib::VertexBuffer::~VertexBuffer() noexcept {

	glDeleteBuffers(1, &glHandle);
}

void glib::VertexBuffer::setData(std::size_t size, void* data, std::uint32_t usage) {

	this->data = data;
	this->size = size;
	this->usage = usage;

	fillBuffer();
}

void glib::VertexBuffer::fillBuffer() {

	glNamedBufferData(glHandle, size, data, usage);
}

void glib::VertexBuffer::updateData(std::uint32_t size, std::uint32_t offset, void* data) {
	glNamedBufferSubData(glHandle, offset, size, data);
}

void glib::VertexBuffer::use() const {
	glBindBuffer(GL_ARRAY_BUFFER, glHandle);
}

void glib::VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}