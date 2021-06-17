#include "globjects/VertexBufferLayout.h"
#include "glad/glad.h"

otg::VertexBufferLayout::VertexBufferLayout() :
	stride(0)
{
	elements.reserve(5);
}

void otg::VertexBufferLayout::addElement(LayoutElement element) {

	element.size = getElementSize(element);

	elements.push_back(element);
	stride += element.size;
}

void otg::VertexBufferLayout::removeElement(std::size_t index) {

	const LayoutElement& element = elements[index];
	auto elementLocation = elements.begin() + index;

	elements.erase(elementLocation);
	stride -= element.size;
}

std::uint32_t otg::VertexBufferLayout::getElementSize(const LayoutElement& element) {

	otg::ElementType type = element.type;
	return getElementTypeSize(type) * element.count;
}

void otg::VertexBufferLayout::applyLayout(std::uint32_t vaoHandle, std::uint32_t vboHandle, std::uint32_t iboHandle) const {

	ArrayHandles handles = {vaoHandle, vboHandle, iboHandle};

	linkBuffer(handles);
	addVertexAttributes(handles);
}

void otg::VertexBufferLayout::linkBuffer(const ArrayHandles& handles) const {

	linkArrayBuffer(handles);
	linkElementBuffer(handles);
}

void otg::VertexBufferLayout::linkArrayBuffer(const ArrayHandles& handles) const {
	glVertexArrayVertexBuffer(handles.vertexArray, 0, handles.arrayBuffer, 0, stride);
}

void otg::VertexBufferLayout::linkElementBuffer(const ArrayHandles& handles) const {
	glVertexArrayElementBuffer(handles.vertexArray, handles.elementBuffer);
}

void otg::VertexBufferLayout::addVertexAttributes(const ArrayHandles& handles) const {

	std::uint32_t offset = 0;

	for (int attribIndex = 0; attribIndex < elements.size(); attribIndex++) {

		const LayoutElement& element = elements[attribIndex];
		VertexAttribute attribute = { element, attribIndex, offset };

		addVertexAttribute(attribute, handles);
		offset += element.size;
	}
}

void otg::VertexBufferLayout::addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handles) const {

	const LayoutElement& element = vertexAttrib.element;
	std::uint32_t glType = getElementGlType(element.type);

	glEnableVertexArrayAttrib(handles.vertexArray, vertexAttrib.index);
	glVertexArrayAttribFormat(handles.vertexArray, vertexAttrib.index, element.count, glType, element.normalized, vertexAttrib.offset);
	glVertexArrayAttribBinding(handles.vertexArray, vertexAttrib.index, 0);
}