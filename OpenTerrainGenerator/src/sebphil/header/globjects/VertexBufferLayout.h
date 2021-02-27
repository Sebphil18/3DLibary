#pragma once
#include <cstdint>
#include <vector>
#include "globjects/LayoutElementType.h"

namespace otg {

	struct LayoutElement {
		ElementType type;
		std::uint32_t count;
		bool normalized;
		std::uint32_t size = 0;
	};

	struct VertexAttribute {
		LayoutElement element;
		std::size_t index;
		std::uint32_t offset;
	};

	struct ArrayHandles {
		std::uint32_t vertexArray;
		std::uint32_t arrayBuffer;
		std::uint32_t elementBuffer;
	};

	class VertexBufferLayout : protected LayoutElementType {

	public:
		VertexBufferLayout();

		void addElement(LayoutElement element);
		void removeElement(std::size_t index);

		void applyLayout(std::uint32_t vao, std::uint32_t vbo, std::uint32_t ibo);

	private:
		std::uint32_t stride;
		std::vector<LayoutElement> elements;

		void linkBuffer(const ArrayHandles& handles);
		void linkArrayBuffer(const ArrayHandles& handles);
		void linkElementBuffer(const ArrayHandles& handles);

		void addVertexAttributes(const ArrayHandles& handles);
		void addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handles);

		std::uint32_t getElementSize(const LayoutElement& element);

	};

}