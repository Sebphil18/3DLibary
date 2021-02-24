#pragma once
#include <string>
#include "globjects/Shader.h"

namespace otg {

	class ShaderProgram {

	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		ShaderProgram(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);

	private:
		std::uint32_t glHandle;

	};

}