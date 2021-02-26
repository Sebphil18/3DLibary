#include "globjects/VertexBufferLayout.h"
#include "glad/glad.h"

otg::VertexBufferLayout::VertexBufferLayout() :
	stride(0)
{
	elements.reserve(5);
}

void otg::VertexBufferLayout::addElement(const LayoutElement& element) {

	elements.push_back(element);
	addElementToStride(element);
}

void otg::VertexBufferLayout::addElementToStride(const LayoutElement& element) {

	std::uint32_t elementSize = getElementSize(element);
	stride += elementSize;
}

void otg::VertexBufferLayout::removeElement(std::size_t index) {

	const LayoutElement& element = elements[index];
	auto elementLocation = elements.begin() + index;

	elements.erase(elementLocation);
	removeElementFromStride(element);
}

void otg::VertexBufferLayout::removeElementFromStride(const LayoutElement& element) {

	std::uint32_t elementSize = getElementSize(element);
	stride -= elementSize;
}

std::uint32_t otg::VertexBufferLayout::getElementSize(const LayoutElement& element) {

	otg::ElementType type = element.type;
	return getElementTypeSize(type) * element.count;
}

void otg::VertexBufferLayout::applyLayout(std::uint32_t vaoHandle, std::uint32_t vboHandle, std::uint32_t iboHandle) {

	ArrayHandles handles = {vaoHandle, vboHandle, iboHandle};

	linkBuffer(handles);
	addVertexAttributes(handles);
}

void otg::VertexBufferLayout::linkBuffer(const ArrayHandles& handles) {

	linkArrayBuffer(handles);
	linkElementBuffer(handles);
}

void otg::VertexBufferLayout::linkArrayBuffer(const ArrayHandles& handles) {
	glVertexArrayVertexBuffer(handles.vertexArray, 0, handles.arrayBuffer, 0, stride);
}

void otg::VertexBufferLayout::linkElementBuffer(const ArrayHandles& handles) {
	glVertexArrayElementBuffer(handles.vertexArray, handles.elementBuffer);
}

void otg::VertexBufferLayout::addVertexAttributes(const ArrayHandles& handles) {

	std::uint32_t offset = 0;

	for (int attribIndex = 0; attribIndex < elements.size(); attribIndex++) {

		const LayoutElement& element = elements[attribIndex];
		VertexAttribute attribute = { element, attribIndex, offset };

		addVertexAttribute(attribute, handles);
	}

}

void otg::VertexBufferLayout::addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handles) {

	const LayoutElement& element = vertexAttrib.element;
	std::uint32_t glType = getElementGlType(element.type);

	glEnableVertexArrayAttrib(handles.vertexArray, vertexAttrib.index);
	glVertexArrayAttribFormat(handles.vertexArray, vertexAttrib.index, element.count, glType, element.normalized, vertexAttrib.offset);
	glVertexArrayAttribBinding(handles.vertexArray, vertexAttrib.index, 0);
}