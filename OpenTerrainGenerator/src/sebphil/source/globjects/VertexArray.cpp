#include "globjects/VertexArray.h"
#include <iostream>
#include "glad/glad.h"

namespace otg {
	VertexArray::VertexArray() noexcept {
		createVertexArray();
	}

	VertexArray::VertexArray(const VertexArray& otherVao) noexcept :
		VertexArray()
	{
	}

	VertexArray& otg::VertexArray::operator=(const VertexArray& otherVao) noexcept {
		
		createVertexArray();

		return *this;
	}

	void otg::VertexArray::createVertexArray() {
		glCreateVertexArrays(1, &glHandle);
	}

	VertexArray::VertexArray(VertexArray&& otherVao) noexcept :
		GlObject(std::move(otherVao))
	{
	}

	VertexArray& VertexArray::operator=(VertexArray&& otherVao) noexcept {

		glHandle = std::move(otherVao.glHandle);
		otherVao.glHandle = 0;

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
}