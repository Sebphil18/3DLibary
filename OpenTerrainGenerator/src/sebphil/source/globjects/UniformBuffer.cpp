#include "globjects/UniformBuffer.h"
#include <iostream>
#include "glad/glad.h"
#include "exceptions/UniformBufferException.h"

namespace otg {

	UniformBuffer::UniformBuffer() noexcept :
		size(0), capacity(64 * 4)
	{
		createRenderBuffer();
	}

	UniformBuffer::UniformBuffer(std::uint32_t capacity) noexcept : 
		size(0), capacity(capacity)
	{
		createRenderBuffer();
		allocateMemory();
	}

	UniformBuffer::UniformBuffer(const UniformBuffer& other) noexcept :
		size(other.size), capacity(other.capacity), elements(other.elements) 	
	{
		createRenderBuffer();
		allocateMemory();
	}

	UniformBuffer& UniformBuffer::operator=(const UniformBuffer& other) noexcept {

		size = other.size;
		capacity = other.capacity;
		elements = other.elements;
		
		createRenderBuffer();
		allocateMemory();

		return *this;
	}

	void UniformBuffer::createRenderBuffer() {
		glCreateBuffers(1, &glHandle);
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

	void UniformBuffer::addElement(const UniformElement& element) {

		elements.push_back({ element.type, element.data, element.size, size });
		size += element.size;

		if (isTooSmall())
			adjustToSize();
		else
			setElementData(elements[elements.size() - 1]);
	}

	bool UniformBuffer::isTooSmall() {
		return capacity < size;
	}

	void UniformBuffer::adjustToSize() {

		capacity = size;
		allocateMemory();

		fillBuffer();
	}

	void UniformBuffer::allocateMemory() {
		glNamedBufferData(glHandle, capacity, nullptr, GL_STATIC_DRAW);
	}

	void UniformBuffer::fillBuffer() {

		for (const UniformElement& element : elements)
			setElementData(element);
	}

	void UniformBuffer::setElementData(std::uint32_t index, void* data) {

		UniformElement& element = elements[index];
		element.data = data;

		setElementData(elements[index]);
	}

	void UniformBuffer::setElementData(const UniformElement& element) {

		if (element.data == nullptr)
			return;

		glNamedBufferSubData(glHandle, element.offset, element.size, element.data);
	}

	void UniformBuffer::bindTo(ShaderProgram& program, const std::string& blockName, std::uint32_t bindingPoint) {

		std::uint32_t index = getBlockIndex(program, blockName);
		
		glUniformBlockBinding(program.getGlHandle(), index, bindingPoint);
		glBindBufferBase(GL_UNIFORM_BUFFER, index, glHandle);
	}

	std::uint32_t otg::UniformBuffer::getBlockIndex(const ShaderProgram& program, const std::string& blockName) {

		std::uint32_t index = glGetUniformBlockIndex(program.getGlHandle(), blockName.c_str());

		try {

			if (index == GL_INVALID_INDEX)
				throw UniformBufferException(blockName);

		} catch (const UniformBufferException& exception) {
			std::cout << exception.what() << "\n";
		}

		return index;
	}

}