#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "globjects/Shader.h"

namespace otg {

	class ShaderProgram {

	public:
		ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		ShaderProgram(const std::string& vertexShaderPath, const std::string& geometryShaderPath, const std::string& fragmentShaderPath);

		~ShaderProgram();

		void use() const;

	private:
		std::uint32_t glHandle;
		std::vector<uint32_t> shaderHandles;

		void buildProgram();

		void attachShaders();

		void linkProgram();
		bool linkingFailed();

		void detachShaders();

		void validateProgram();
		bool validationFailed();

	};

}