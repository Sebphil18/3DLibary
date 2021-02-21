#include "globjects/IndexBuffer.h"
#include <memory>
#include "glad/glad.h"

otg::IndexBuffer::IndexBuffer() noexcept :
	usage(GL_STATIC_DRAW), size(0), data(nullptr)
{
	createBuffer();
}

otg::IndexBuffer::IndexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept :
	usage(usage), size(size), data(data)
{
	createBuffer();
	fillBuffer();
}

otg::IndexBuffer::IndexBuffer(const IndexBuffer& otherIbo) noexcept :
	usage(otherIbo.usage), size(otherIbo.size), data(otherIbo.data)
{
	createBuffer();
	fillBuffer();
}

otg::IndexBuffer::IndexBuffer(IndexBuffer&& otherIbo) noexcept:
	glHandle(std::move(otherIbo.glHandle)), 
	usage(std::move(otherIbo.usage)), 
	size(std::move(otherIbo.size)), 
	data(std::move(otherIbo.data))
{
}

otg::IndexBuffer& otg::IndexBuffer::operator=(const IndexBuffer& otherIbo) noexcept {

	copyMembers(otherIbo);
	createBuffer();
	fillBuffer();

	return *this;
}

void otg::IndexBuffer::copyMembers(const IndexBuffer& otherIbo) {

	usage = otherIbo.usage;
	size = otherIbo.size;
	data = otherIbo.data;
}

void otg::IndexBuffer::createBuffer() {

	glCreateBuffers(1, &glHandle);
}

otg::IndexBuffer& otg::IndexBuffer::operator=(IndexBuffer&& otherIbo) noexcept {
	
	moveMembers(otherIbo);

	return *this;
}

void otg::IndexBuffer::moveMembers(IndexBuffer& otherIbo) {

	glHandle = std::move(otherIbo.glHandle);
	usage = std::move(otherIbo.usage);
	size = std::move(otherIbo.size);
	data = std::move(otherIbo.data);
}

otg::IndexBuffer::~IndexBuffer() noexcept {
	deleteBuffer();
}

void otg::IndexBuffer::deleteBuffer() {

	glDeleteBuffers(1, &glHandle);
}

void otg::IndexBuffer::setData(std::size_t size, void* data, std::uint32_t usage) {
	
	this->usage = usage;
	this->size = size;
	this->data = data;

	fillBuffer();
}

void otg::IndexBuffer::fillBuffer() {

	glNamedBufferData(glHandle, size, data, usage);
}

std::uint32_t otg::IndexBuffer::getGlHandle() const {
	return glHandle;
}