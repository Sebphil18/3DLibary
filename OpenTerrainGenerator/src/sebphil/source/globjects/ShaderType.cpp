#include "globjects/ShaderType.h"
#include "glad/glad.h"

namespace otg {

	std::int32_t otg::ShaderStageType::getGlType(ShaderType type) {

		switch (type) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;

		case ShaderType::Geometry:
			return GL_GEOMETRY_SHADER;

		case ShaderType::Tesellation:
			return GL_TESS_CONTROL_SHADER;

		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;

		case ShaderType::Compute:
			return GL_COMPUTE_SHADER;

		default:
			return 0;

		}
	}

}