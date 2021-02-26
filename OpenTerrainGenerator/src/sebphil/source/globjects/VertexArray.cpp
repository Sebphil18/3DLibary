#include "globjects/VertexArray.h"
#include <iostream>
#include "glad/glad.h"

namespace otg {
	VertexArray::VertexArray() noexcept {
		createVertexArray();
	}

	void otg::VertexArray::createVertexArray() {
		glCreateVertexArrays(1, &glHandle);
	}

	VertexArray::VertexArray(VertexArray&& otherVertArray) noexcept :
		glHandle(std::move(otherVertArray.glHandle)) {
	}

	VertexArray& VertexArray::operator=(VertexArray&& otherVertArray) noexcept {

		glHandle = std::move(otherVertArray.glHandle);

		return *this;
	}

	VertexArray::~VertexArray() noexcept {

		deleteVertexArray();
	}

	void VertexArray::deleteVertexArray() {
		glDeleteVertexArrays(1, &glHandle);
	}

	void VertexArray::use() {
		glBindVertexArray(glHandle);
	}

	std::uint32_t VertexArray::getGlHandle() const {
		return glHandle;
	}
}