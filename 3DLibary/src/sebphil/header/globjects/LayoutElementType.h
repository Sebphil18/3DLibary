#pragma once
#include <cstdint>

namespace otg {

	class LayoutElementType {

	public:
		enum class ElementType {
			Int, Uint, Float, Double, Short, Byte, Bool
		};

	protected:
		static std::uint32_t getElementTypeSize(ElementType type);
		static std::uint32_t getElementGlType(ElementType type);

	};

	typedef LayoutElementType::ElementType ElementType;

}
