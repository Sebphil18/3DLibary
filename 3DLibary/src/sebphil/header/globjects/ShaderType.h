#pragma once
#include <cstdint>

namespace glib {

	class ShaderStageType {

	public:
		enum class ShaderType {
			Vertex, Geometry, Fragment
		};

		static std::int32_t getGlType(ShaderType type);

	};

	typedef ShaderStageType::ShaderType ShaderType;

}