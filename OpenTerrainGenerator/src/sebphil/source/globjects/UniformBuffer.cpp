#include "globjects/UniformBuffer.h"
#include <iostream>
#include "glad/glad.h"
#include "exceptions/UniformBufferException.h"

namespace otg {

	UniformBuffer::UniformBuffer() noexcept : 
		size(0)
	{
		createRenderBuffer();
	}

	UniformBuffer::UniformBuffer(const UniformBuffer& other) noexcept :
		size(other.size), elements(other.elements) 	
	{
		createRenderBuffer();
	}

	UniformBuffer& UniformBuffer::operator=(const UniformBuffer& other) noexcept {

		size = other.size;
		elements = other.elements;
		
		createRenderBuffer();

		return *this;
	}

	void UniformBuffer::createRenderBuffer() {
		glCreateBuffers(1, &glHandle);
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : 
		size(std::move(other.size)),
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

		size += element.size;
		elements.push_back(element);
	}

	void UniformBuffer::setElement(std::size_t index, UniformElement element) {
		
		size -= element.size;

		elements[index] = element;
		size += element.size;
	}

	void UniformBuffer::removeElement(std::size_t index) {

		size -= elements[index].size;
		elements.erase(elements.begin() + index);
	}

	void UniformBuffer::fillBuffer() {

		glNamedBufferData(glHandle, size, nullptr, GL_STATIC_DRAW);

		std::size_t offset = 0;
		for (std::size_t i = 0; i < elements.size(); i++) {
			
			const UniformElement& element = elements[i];

			glNamedBufferSubData(glHandle, offset, element.size, element.data);

			offset += element.size;
		}
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