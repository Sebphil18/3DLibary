#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "globjects/GlObject.h"
#include "globjects/Shader.h"
#include "globjects/ProgramUniformLink.h"

namespace otg {

	class ShaderProgram : public GlObject, public ProgramUniformLink {

	public:
		ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		ShaderProgram(const std::string& vertexShaderPath, const std::string& geometryShaderPath, const std::string& fragmentShaderPath);

		~ShaderProgram();

		void use() const;

		void setUniformBlockBinding(std::uint32_t index, std::uint32_t bindingPoint);
		void setUniformBlockBinding(const std::string& blockName, std::uint32_t bindingPoint);

	private:
		std::vector<uint32_t> shaderHandles;

		void buildProgram();
		void buildExecutable();

		void attachShaders();

		void linkProgram();
		bool linkingFailed();

		void detachShaders();

		void validateProgram();
		bool validationFailed();

		std::uint32_t getBlockIndex(const std::string& blockName);

	};

}