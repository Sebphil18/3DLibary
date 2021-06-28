#include "globjects/VertexBufferLayout.h"
#include "glad/glad.h"

glib::VertexBufferLayout::VertexBufferLayout() :
	stride(0)
{
	elements.reserve(5);
}

void glib::VertexBufferLayout::addElement(LayoutElement element) {

	element.size = getElementSize(element);

	elements.push_back(element);
	stride += element.size;
}

void glib::VertexBufferLayout::removeElement(std::size_t index) {

	const LayoutElement& element = elements[index];
	auto elementLocation = elements.begin() + index;

	elements.erase(elementLocation);
	stride -= element.size;
}

std::uint32_t glib::VertexBufferLayout::getElementSize(const LayoutElement& element) {

	glib::ElementType type = element.type;
	return getElementTypeSize(type) * element.count;
}

void glib::VertexBufferLayout::applyLayout(std::uint32_t vaoHandle, std::uint32_t vboHandle, std::uint32_t iboHandle) const {

	ArrayHandles handles = {vaoHandle, vboHandle, iboHandle};

	linkBuffer(handles);
	addVertexAttributes(handles);
}

void glib::VertexBufferLayout::linkBuffer(const ArrayHandles& handles) const {

	linkArrayBuffer(handles);
	linkElementBuffer(handles);
}

void glib::VertexBufferLayout::linkArrayBuffer(const ArrayHandles& handles) const {
	glVertexArrayVertexBuffer(handles.vertexArray, 0, handles.arrayBuffer, 0, stride);
}

void glib::VertexBufferLayout::linkElementBuffer(const ArrayHandles& handles) const {
	glVertexArrayElementBuffer(handles.vertexArray, handles.elementBuffer);
}

void glib::VertexBufferLayout::addVertexAttributes(const ArrayHandles& handles) const {

	std::uint32_t offset = 0;

	for (int attribIndex = 0; attribIndex < elements.size(); attribIndex++) {

		const LayoutElement& element = elements[attribIndex];
		VertexAttribute attribute = { element, attribIndex, offset };

		addVertexAttribute(attribute, handles);
		offset += element.size;
	}
}

void glib::VertexBufferLayout::addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handles) const {

	const LayoutElement& element = vertexAttrib.element;
	std::uint32_t glType = getElementGlType(element.type);

	glEnableVertexArrayAttrib(handles.vertexArray, vertexAttrib.index);
	glVertexArrayAttribFormat(handles.vertexArray, vertexAttrib.index, element.count, glType, element.normalized, vertexAttrib.offset);
	glVertexArrayAttribBinding(handles.vertexArray, vertexAttrib.index, 0);
}