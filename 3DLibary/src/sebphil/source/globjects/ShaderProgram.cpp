#include "globjects/ShaderProgram.h"
#include "glad/glad.h"
#include "exceptions/ProgramBuildException.h"
#include "exceptions/UniformBufferException.h"

namespace glib {

	// TDOO: duplicated code
	ShaderProgram::ShaderProgram(const SourceBuild& vertexShaderSrc, const SourceBuild& fragmentShaderSrc) {

		glHandle = glCreateProgram();
		setProgramUniformHandle(glHandle);

		Shader vertexShader(vertexShaderSrc, ShaderType::Vertex);
		Shader fragmentShader(fragmentShaderSrc, ShaderType::Fragment);

		shaderHandles.push_back(vertexShader.getGlHandle());
		shaderHandles.push_back(fragmentShader.getGlHandle());

		buildProgram();

	}

	ShaderProgram::ShaderProgram(const SourceBuild& vertexShaderSrc, const SourceBuild& geometryShaderSrc, const SourceBuild& fragmentShaderSrc) {

		glHandle = glCreateProgram();
		setProgramUniformHandle(glHandle);

		Shader vertexShader(vertexShaderSrc, ShaderType::Vertex);
		Shader fragmentShader(geometryShaderSrc, ShaderType::Fragment);
		Shader geometryShader(fragmentShaderSrc, ShaderType::Geometry);

		shaderHandles.push_back(vertexShader.getGlHandle());
		shaderHandles.push_back(fragmentShader.getGlHandle());
		shaderHandles.push_back(geometryShader.getGlHandle());

		buildProgram();
	}

	void ShaderProgram::buildProgram() {

		attachShaders();
		buildExecutable();
		detachShaders();
	}

	void ShaderProgram::attachShaders() {

		for (std::uint32_t shaderHandle : shaderHandles)
			glAttachShader(glHandle, shaderHandle);
	}

	void ShaderProgram::buildExecutable() {

		try {

			linkProgram();
			validateProgram();

		} catch (const ProgramBuildException& exception) {
			std::cout << exception.what() << "\n";
		}
	}

	void ShaderProgram::linkProgram() {

		glLinkProgram(glHandle);

		if (linkingFailed())
			throw ProgramBuildException(glHandle);
	}

	bool ShaderProgram::linkingFailed() {
		
		std::int32_t success;
		glGetProgramiv(glHandle, GL_LINK_STATUS, &success);

		return !success;
	}

	void ShaderProgram::validateProgram() {

		glValidateProgram(glHandle);

		if (validationFailed())
			throw ProgramBuildException(glHandle);
	}

	bool ShaderProgram::validationFailed() {

		std::int32_t isValid;
		glGetProgramiv(glHandle, GL_VALIDATE_STATUS, &isValid);

		return !isValid;
	}

	void ShaderProgram::detachShaders() {

		for (std::uint32_t shaderHandle : shaderHandles)
			glDetachShader(glHandle, shaderHandle);
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(glHandle);
	}

	void ShaderProgram::use() const {
		glUseProgram(glHandle);
	}

	void ShaderProgram::setUniformBlockBinding(std::uint32_t index, std::uint32_t bindingPoint) {
		glUniformBlockBinding(glHandle, index, bindingPoint);
	}

	void ShaderProgram::setUniformBlockBinding(const std::string& blockName, std::uint32_t bindingPoint) {

		std::uint32_t index = getBlockIndex(blockName);
		glUniformBlockBinding(glHandle, index, bindingPoint);
	}

	std::uint32_t ShaderProgram::getBlockIndex(const std::string& blockName) {

		std::uint32_t index = glGetUniformBlockIndex(glHandle, blockName.c_str());

		try {

			if (index == GL_INVALID_INDEX)
				throw UniformBufferException(blockName);

		} catch (const UniformBufferException& exception) {
			std::cout << exception.what() << "\n";
		}

		return index;
	}

}