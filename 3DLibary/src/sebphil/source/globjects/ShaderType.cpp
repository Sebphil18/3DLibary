#include "globjects/ShaderType.h"
#include "glad/glad.h"

namespace glib {

	std::int32_t glib::ShaderStageType::getGlType(ShaderType type) {

		switch (type) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;

		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;

		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;

		default:
			return 0;

		}
	}

}