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
	init();
}

otg::IndexBuffer::IndexBuffer(const IndexBuffer& otherIbo) noexcept :
	IndexBuffer(otherIbo.size, otherIbo.data, otherIbo.usage)
{
}

otg::IndexBuffer& otg::IndexBuffer::operator=(const IndexBuffer& otherIbo) noexcept {

	usage = otherIbo.usage;
	size = otherIbo.size;
	data = otherIbo.data;

	init();

	return *this;
}

void otg::IndexBuffer::init() {
	createBuffer();
	fillBuffer();
}

void otg::IndexBuffer::createBuffer() {

	glCreateBuffers(1, &glHandle);
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

otg::IndexBuffer::IndexBuffer(IndexBuffer&& otherIbo) noexcept : 
	GlObject(std::move(otherIbo)),
	usage(std::move(otherIbo.usage)),
	size(std::move(otherIbo.size)),
	data(std::move(otherIbo.data))
{
}

otg::IndexBuffer& otg::IndexBuffer::operator=(IndexBuffer&& otherIbo) noexcept {
	
	GlObject::operator=(std::move(otherIbo));

	usage = std::move(otherIbo.usage);
	size = std::move(otherIbo.size);
	data = std::move(otherIbo.data);

	return *this;
}

otg::IndexBuffer::~IndexBuffer() noexcept {
	deleteBuffer();
}

void otg::IndexBuffer::deleteBuffer() {

	glDeleteBuffers(1, &glHandle);
}

std::size_t otg::IndexBuffer::getElementCount() const {
	return size / sizeof(int);
}