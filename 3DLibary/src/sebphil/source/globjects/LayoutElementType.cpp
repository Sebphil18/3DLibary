#include "globjects/LayoutElementType.h"
#include "glad/glad.h"

std::uint32_t glib::LayoutElementType::getElementTypeSize(ElementType type) {

	switch (type) {

	case ElementType::Int:
		return 4;

	case ElementType::Uint:
		return 4;

	case ElementType::Float:
		return 4;

	case ElementType::Double:
		return 8;

	case ElementType::Short:
		return 2;

	case ElementType::Byte:
		return 1;

	case ElementType::Bool:
		return 1;

	default:
		return 0;

	}
}

std::uint32_t glib::LayoutElementType::getElementGlType(ElementType type) {

	switch (type) {

	case ElementType::Int:
		return GL_INT;

	case ElementType::Uint:
		return GL_UNSIGNED_INT;

	case ElementType::Float:
		return GL_FLOAT;

	case ElementType::Double:
		return GL_DOUBLE;

	case ElementType::Short:
		return GL_SHORT;

	case ElementType::Byte:
		return GL_BYTE;

	case ElementType::Bool:
		return GL_BOOL;

	default:
		return 0;

	}
}