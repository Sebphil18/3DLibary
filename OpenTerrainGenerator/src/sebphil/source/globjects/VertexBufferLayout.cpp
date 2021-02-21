#include "globjects/VertexBufferLayout.h"
#include "glad/glad.h"

otg::VertexBufferLayout::VertexBufferLayout():
	stride(0)
{
	elements.reserve(5);
}

void otg::VertexBufferLayout::addElement(LayoutElement element) {

	element.size = getElementSize(element);

	elements.push_back(element);
	stride += element.size;
}

std::uint32_t otg::VertexBufferLayout::getElementSize(const LayoutElement& element) {

	if (element.size == 0) {

		otg::ElementType type = element.type;
		return getElementTypeSize(type) * element.count;

	} else {

		return element.size;
	}
}

void otg::VertexBufferLayout::removeElement(std::size_t index) {

	const LayoutElement& element = elements[index];
	auto elementLocation = elements.begin() + index;

	elements.erase(elementLocation);
	stride -= element.size;
}

void otg::VertexBufferLayout::applyLayout(std::uint32_t vao, std::uint32_t vbo, std::uint32_t ibo) {

	ArrayHandels handels = {vao, vbo, ibo};

	linkBuffer(handels);

	std::uint32_t offset = 0;

	for (int attribIndex = 0; attribIndex < elements.size(); attribIndex++) {

		const LayoutElement& element = elements[attribIndex];
		VertexAttribute attribute = {element, attribIndex, offset};

		addVertexAttribute(attribute, handels);
	}
}

void otg::VertexBufferLayout::linkBuffer(const ArrayHandels& handels) {

	linkArrayBuffer(handels);
	linkElementBuffer(handels);
}

void otg::VertexBufferLayout::linkArrayBuffer(const ArrayHandels& handels) {

	glVertexArrayVertexBuffer(handels.vertexArray, 0, handels.arrayBuffer, 0, stride);
}

void otg::VertexBufferLayout::linkElementBuffer(const ArrayHandels& handels) {

	glVertexArrayElementBuffer(handels.vertexArray, handels.elementBuffer);
}

void otg::VertexBufferLayout::addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandels& handels) {

	const LayoutElement& element = vertexAttrib.element;
	std::uint32_t glType = getElementGlType(element.type);

	glVertexArrayAttribFormat(handels.vertexArray, vertexAttrib.index, element.count, glType, element.normalized, vertexAttrib.offset);
	glVertexArrayAttribBinding(handels.vertexArray, vertexAttrib.index, 0);
	glEnableVertexArrayAttrib(handels.vertexArray, vertexAttrib.index);
}