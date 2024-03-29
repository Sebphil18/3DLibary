#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "globjects/GlObject.h"
#include "globjects/Shader.h"
#include "globjects/ProgramUniformLink.h"
#include "preprocessor/SourceBuild.h"

namespace glib {

	class ShaderProgram : public GlObject, public ProgramUniformLink {

	public:
		ShaderProgram(const SourceBuild& vertexShaderSrc, const SourceBuild& fragmentShaderSrc);
		ShaderProgram(const SourceBuild& vertexShaderSrc, const SourceBuild& geometryShaderSrc, const SourceBuild& fragmentShaderSrc);

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