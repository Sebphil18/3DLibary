#include "globjects/VertexBuffer.h"
#include <iostream>
#include "glad/glad.h"

otg::VertexBuffer::VertexBuffer() noexcept :
	usage(GL_STATIC_DRAW), size(0), data(nullptr)
{
	createBuffer();
	fillBuffer();
}

otg::VertexBuffer::VertexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept :
	usage(usage), size(size), data(data)
{
	createBuffer();
	fillBuffer();
}

otg::VertexBuffer::VertexBuffer(const VertexBuffer& otherVbo) noexcept :
	VertexBuffer(otherVbo.size, otherVbo.data, otherVbo.usage)
{
}

otg::VertexBuffer& otg::VertexBuffer::operator=(const VertexBuffer& otherVbo) noexcept {

	usage = otherVbo.usage;
	size = otherVbo.size;
	data = otherVbo.data;

	fillBuffer();

	return *this;
}

void otg::VertexBuffer::createBuffer() {

	glCreateBuffers(1, &glHandle);
}

otg::VertexBuffer::VertexBuffer(VertexBuffer&& otherVbo) noexcept :
	GlObject(std::move(otherVbo)),
	usage(std::move(otherVbo.usage)),
	size(std::move(otherVbo.size)),
	data(std::move(otherVbo.data))
{
}

otg::VertexBuffer& otg::VertexBuffer::operator=(VertexBuffer&& otherVbo) noexcept {

	GlObject::operator=(std::move(otherVbo));

	usage = std::move(otherVbo.usage);
	size = std::move(otherVbo.size);
	data = std::move(otherVbo.data);

	return *this;
}

otg::VertexBuffer::~VertexBuffer() noexcept {

	glDeleteBuffers(1, &glHandle);
}

void otg::VertexBuffer::setData(std::size_t size, void* data, std::uint32_t usage) {

	this->data = data;
	this->size = size;
	this->usage = usage;

	fillBuffer();
}

void otg::VertexBuffer::fillBuffer() {

	glNamedBufferData(glHandle, size, data, usage);
}

void otg::VertexBuffer::updateData(std::uint32_t size, std::uint32_t offset, void* data) {
	glNamedBufferSubData(glHandle, offset, size, data);
}

void otg::VertexBuffer::use() const {
	glBindBuffer(GL_ARRAY_BUFFER, glHandle);
}

void otg::VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}