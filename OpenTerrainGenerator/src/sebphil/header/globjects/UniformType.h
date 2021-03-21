#pragma once
#include <cstdint>

namespace otg {

	class UniformTypes {

	public:

		enum class UniformType {
			Number, Vector2, Vector3, Vector4, Matrix4
		};

		UniformTypes(UniformType type) noexcept;

		static std::uint32_t getSize(UniformType type);

	protected:
		UniformType type;
	};

	typedef UniformTypes::UniformType UniformType;
}
