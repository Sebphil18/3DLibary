#include "globjects/UniformType.h"

namespace otg {

	UniformTypes::UniformTypes(UniformType type) noexcept : 
		type(type)
	{
	}

	std::uint32_t UniformTypes::getSize(UniformType type) {
		
		switch (type) {

		case UniformType::Number:
			return 4;

		case UniformType::Vector2:
			return 4 * 2;

		case UniformType::Vector3:
			return 4 * 4;

		case UniformType::Vector4:
			return 4 * 4;

		case UniformType::Matrix4:
			return 4 * 4 * 4;

		}

		return 0;
	}

}