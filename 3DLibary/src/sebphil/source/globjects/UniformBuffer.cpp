#include "globjects/UniformBuffer.h"
#include "glad/glad.h"

namespace glib {

	UniformBuffer::UniformBuffer() noexcept :
		size(0), capacity(64 * 4)
	{
		createBuffer();
		adjustMemory();
	}

	UniformBuffer::UniformBuffer(std::uint32_t capacity) noexcept : 
		size(0), capacity(capacity)
	{
		createBuffer();
		adjustMemory();
	}

	UniformBuffer::UniformBuffer(const UniformBuffer& other) noexcept :
		size(other.size), capacity(other.capacity), elements(other.elements) 	
	{
		createBuffer();
		adjustMemory();
		fillBuffer();
	}

	UniformBuffer& UniformBuffer::operator=(const UniformBuffer& other) noexcept {

		size = other.size;
		capacity = other.capacity;
		elements = other.elements;
		
		adjustMemory();
		fillBuffer();

		return *this;
	}

	void UniformBuffer::createBuffer() {
		glCreateBuffers(1, &glHandle);
	}

	void UniformBuffer::addElement(const UniformElement& element) {

		elements.push_back({ element.type, element.data, element.size, size });
		size += element.size;

		adjustBuffer();
	}

	void UniformBuffer::adjustBuffer() {

		if (capacity < size) {

			capacity = size;

			adjustMemory();
			fillBuffer();
		}
	}

	void UniformBuffer::adjustMemory() {
		glNamedBufferData(glHandle, capacity, nullptr, GL_STATIC_DRAW);
	}

	void UniformBuffer::fillBuffer() {

		for (const UniformElement& element : elements)
			glNamedBufferSubData(glHandle, element.offset, element.size, element.data);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : 
		size(std::move(other.size)),
		capacity(std::move(other.capacity)),
		elements(std::move(other.elements)),
		GlObject(std::move(other))
	{
	}

	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept {

		GlObject::operator=(std::move(other));

		size = std::move(other.size);
		elements = std::move(other.elements);

		return *this;
	}

	UniformBuffer::~UniformBuffer() noexcept {
		glDeleteBuffers(1, &glHandle);
	}

	void UniformBuffer::setElementData(std::uint32_t index, void* data) {

		UniformElement& element = elements[index];

		element.data = data;
		glNamedBufferSubData(glHandle, element.offset, element.size, element.data);
	}

	void UniformBuffer::bindTo(std::uint32_t bindingPoint) {
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, glHandle);
	}

}