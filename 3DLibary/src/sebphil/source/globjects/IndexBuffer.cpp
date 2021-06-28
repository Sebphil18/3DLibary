#include "globjects/IndexBuffer.h"
#include <memory>
#include "glad/glad.h"

namespace glib {

	IndexBuffer::IndexBuffer() noexcept :
		usage(GL_STATIC_DRAW), size(0), data(nullptr) {
		createBuffer();
	}

	IndexBuffer::IndexBuffer(std::size_t size, void* data, std::uint32_t usage) noexcept :
		usage(usage), size(size), data(data) {
		init();
	}

	void IndexBuffer::init() {
		createBuffer();
		fillBuffer();
	}

	void IndexBuffer::createBuffer() {

		glCreateBuffers(1, &glHandle);
	}

	IndexBuffer::IndexBuffer(const IndexBuffer& otherIbo) noexcept :
		IndexBuffer(otherIbo.size, otherIbo.data, otherIbo.usage) {
	}

	IndexBuffer& IndexBuffer::operator=(const IndexBuffer& otherIbo) noexcept {

		usage = otherIbo.usage;
		size = otherIbo.size;
		data = otherIbo.data;

		fillBuffer();

		return *this;
	}

	void IndexBuffer::setData(std::size_t size, void* data, std::uint32_t usage) {

		this->usage = usage;
		this->size = size;
		this->data = data;

		fillBuffer();
	}

	void IndexBuffer::fillBuffer() {

		glNamedBufferData(glHandle, size, data, usage);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& otherIbo) noexcept :
		GlObject(std::move(otherIbo)),
		usage(std::move(otherIbo.usage)),
		size(std::move(otherIbo.size)),
		data(std::move(otherIbo.data)) {
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& otherIbo) noexcept {

		GlObject::operator=(std::move(otherIbo));

		usage = std::move(otherIbo.usage);
		size = std::move(otherIbo.size);
		data = std::move(otherIbo.data);

		return *this;
	}

	IndexBuffer::~IndexBuffer() noexcept {
		deleteBuffer();
	}

	void IndexBuffer::deleteBuffer() {

		glDeleteBuffers(1, &glHandle);
	}

	std::size_t IndexBuffer::getElementCount() const {
		return size / sizeof(int);
	}
}