#include "globjects/VertexBufferLayout.h"
#include "glad/glad.h"

otg::VertexBufferLayout::VertexBufferLayout() :
	stride(0)
{
	elements.reserve(5);
}

void otg::VertexBufferLayout::addElement(const LayoutElement& element) {

	elements.push_back(element);
	stride += element.size;
}

void otg::VertexBufferLayout::removeElement(std::size_t index) {

	const LayoutElement& element = elements[index];
	auto elementLocation = elements.begin() + index;

	elements.erase(elementLocation);
	stride -= element.size;
}

void otg::VertexBufferLayout::applyLayout(std::uint32_t vao, std::uint32_t vbo, std::uint32_t ibo) {

	ArrayHandles handels = {vao, vbo, ibo};

	linkBuffer(handels);
	addVertexAttributes(handels);
}

void otg::VertexBufferLayout::linkBuffer(const ArrayHandles& handels) {

	linkArrayBuffer(handels);
	linkElementBuffer(handels);
}

void otg::VertexBufferLayout::linkArrayBuffer(const ArrayHandles& handels) {
	glVertexArrayVertexBuffer(handels.vertexArray, 0, handels.arrayBuffer, 0, stride);
}

void otg::VertexBufferLayout::linkElementBuffer(const ArrayHandles& handels) {
	glVertexArrayElementBuffer(handels.vertexArray, handels.elementBuffer);
}

void otg::VertexBufferLayout::addVertexAttributes(const ArrayHandles& handels) {

	std::uint32_t offset = 0;

	for (int attribIndex = 0; attribIndex < elements.size(); attribIndex++) {

		const LayoutElement& element = elements[attribIndex];
		VertexAttribute attribute = { element, attribIndex, offset };

		addVertexAttribute(attribute, handels);
	}
}

void otg::VertexBufferLayout::addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handels) {

	const LayoutElement& element = vertexAttrib.element;
	std::uint32_t glType = getElementGlType(element.type);

	glVertexArrayAttribFormat(handels.vertexArray, vertexAttrib.index, element.count, glType, element.normalized, vertexAttrib.offset);
	glVertexArrayAttribBinding(handels.vertexArray, vertexAttrib.index, 0);
	glEnableVertexArrayAttrib(handels.vertexArray, vertexAttrib.index);
}

std::uint32_t otg::VertexBufferLayout::getElementSize(const LayoutElement& element) {

	otg::ElementType type = element.type;
	return getElementTypeSize(type) * element.count;
}