#pragma once
#include <cstdint>
#include <vector>
#include "globjects/LayoutElementType.h"

namespace otg {

	struct LayoutElement {
		ElementType type;
		std::uint32_t count;
		bool normalized = false;
		std::uint32_t size = 0;
	};

	struct VertexAttribute {
		LayoutElement element;
		std::int32_t index;
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

		void applyLayout(std::uint32_t vao, std::uint32_t vbo, std::uint32_t ibo) const;

	private:
		std::uint32_t stride;
		std::vector<LayoutElement> elements;

		void linkBuffer(const ArrayHandles& handles) const;
		void linkArrayBuffer(const ArrayHandles& handles) const;
		void linkElementBuffer(const ArrayHandles& handles) const;

		void addVertexAttributes(const ArrayHandles& handles) const;
		void addVertexAttribute(const VertexAttribute& vertexAttrib, const ArrayHandles& handles) const;

		std::uint32_t getElementSize(const LayoutElement& element);

	};

}