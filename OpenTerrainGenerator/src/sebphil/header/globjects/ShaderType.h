#pragma once
#include <cstdint>

namespace otg {

	class ShaderStageType {

	public:
		enum class ShaderType {
			Vertex, Geometry, Tesellation, Fragment, Compute
		};

		static std::int32_t getGlType(ShaderType type);

	};

	typedef ShaderStageType::ShaderType ShaderType;

}