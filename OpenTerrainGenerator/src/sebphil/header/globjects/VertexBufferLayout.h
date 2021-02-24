#pragma once
#include <cstdint>
#include <vector>
#include "globjects/LayoutElementType.h"

namespace otg {

	// Move into LayoutElementType.h
	typedef LayoutElementType::ElementType ElementType;

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

		void addElement(const LayoutElement& element);
		void removeElement(std::size_t index);

		void applyLayout(std::uint32_t vao, std::uint32_t vbo, std::uint32_t ibo);

		std::uint32_t getElementSize(const LayoutElement& element);

	private:
		std::uint32_t stride;
		std::vector<LayoutElement> elements;

		void linkBuffer(const ArrayHandles& handels);
		void linkArrayBuffer(const ArrayHandles& handels);
		void linkElementBuffer(const ArrayHandles& handels);

		void addVertexAttributes(const ArrayHandles& handels);
		void addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handels);

	};

}