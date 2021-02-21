#include "globjects/VertexArray.h"
#include <iostream>
#include "glad/glad.h"

otg::VertexArray::VertexArray() noexcept {
	createVertexArray();
}

otg::VertexArray::VertexArray(const VertexArray& otherVertArray) noexcept {
	createVertexArray();
}

otg::VertexArray::VertexArray(VertexArray&& otherVertArray) noexcept :
	glHandle(std::move(otherVertArray.glHandle))
{
}

otg::VertexArray& otg::VertexArray::operator=(const VertexArray& otherVertArray) noexcept {

	createVertexArray();

	return *this;
}

void otg::VertexArray::createVertexArray() {
	glCreateVertexArrays(1, &glHandle);
}

otg::VertexArray& otg::VertexArray::operator=(VertexArray&& otherVertArray) noexcept {

	glHandle = std::move(otherVertArray.glHandle);

	return *this;
}

otg::VertexArray::~VertexArray() noexcept {

	deleteVertexArray();
}

void otg::VertexArray::deleteVertexArray() {
	glDeleteVertexArrays(1, &glHandle);
}

void otg::VertexArray::bind() {
	glBindVertexArray(glHandle);
}

std::uint32_t otg::VertexArray::getGlHandle() const {
	return glHandle;
}