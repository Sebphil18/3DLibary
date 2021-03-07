#include "globjects/VertexBuffer.h"
#include <iostream>
#include "glad/glad.h"

otg::VertexBuffer::VertexBuffer() noexcept :
	data(nullptr), size(0), usage(GL_STATIC_DRAW)
{
	createBuffer();
}

otg::VertexBuffer::VertexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept :
	data(data), size(size), usage(usage)
{
	createBuffer();
	fillBuffer();
}

otg::VertexBuffer::VertexBuffer(const VertexBuffer& otherObj) noexcept :
	data(otherObj.data), size(otherObj.size), usage(otherObj.usage)
{
	createBuffer();
	fillBuffer();
}

otg::VertexBuffer::VertexBuffer(VertexBuffer&& otherObj) noexcept {
	moveMember(otherObj);
}

otg::VertexBuffer& otg::VertexBuffer::operator=(const VertexBuffer& otherObj) noexcept {

	data = otherObj.data;
	size = otherObj.size;
	usage = otherObj.usage;

	createBuffer();
	fillBuffer();

	return *this;
}

void otg::VertexBuffer::createBuffer() {

	glCreateBuffers(1, &glHandle);
}

otg::VertexBuffer& otg::VertexBuffer::operator=(VertexBuffer&& otherObj) noexcept {

	moveMember(otherObj);

	return *this;
}

void otg::VertexBuffer::moveMember(VertexBuffer& otherObj) noexcept {

	data = std::move(otherObj.data);
	size = std::move(otherObj.size);
	usage = std::move(otherObj.usage);
	glHandle = std::move(otherObj.glHandle);
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