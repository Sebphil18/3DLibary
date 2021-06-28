#include "globjects/VertexArray.h"
#include <iostream>
#include "glad/glad.h"

namespace glib {
	VertexArray::VertexArray() noexcept {
		createVertexArray();
	}

	VertexArray::VertexArray(const VertexArray& otherVao) noexcept :
		VertexArray()
	{
	}

	VertexArray& glib::VertexArray::operator=(const VertexArray& otherVao) noexcept {

		return *this;
	}

	void glib::VertexArray::createVertexArray() {
		glCreateVertexArrays(1, &glHandle);
	}

	VertexArray::VertexArray(VertexArray&& otherVao) noexcept :
		GlObject(std::move(otherVao))
	{
	}

	VertexArray& VertexArray::operator=(VertexArray&& otherVao) noexcept {

		GlObject::operator=(std::move(otherVao));

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