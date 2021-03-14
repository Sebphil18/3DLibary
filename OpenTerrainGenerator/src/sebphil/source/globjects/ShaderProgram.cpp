#include "globjects/ShaderProgram.h"
#include "glad/glad.h"
#include "exceptions/ProgramBuildException.h"

namespace otg {

	// TDOO: duplicated code
	ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {

		glHandle = glCreateProgram();
		setProgramUniformHandle(glHandle);

		Shader vertexShader(vertexShaderPath, ShaderType::Vertex);
		Shader fragmentShader(fragmentShaderPath, ShaderType::Fragment);

		shaderHandles.push_back(vertexShader.getGlHandle());
		shaderHandles.push_back(fragmentShader.getGlHandle());

		buildProgram();
	}

	ShaderProgram::ShaderProgram(const std::string& vertexShaderPath, const std::string& geometryShaderPath, const std::string& fragmentShaderPath) {

		glHandle = glCreateProgram();
		setProgramUniformHandle(glHandle);

		Shader vertexShader(vertexShaderPath, ShaderType::Vertex);
		Shader fragmentShader(fragmentShaderPath, ShaderType::Fragment);
		Shader geometryShader(geometryShaderPath, ShaderType::Geometry);

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

	void otg::ShaderProgram::buildExecutable() {

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
			throw otg::ProgramBuildException(glHandle);
	}

	bool otg::ShaderProgram::validationFailed() {

		std::int32_t isValid;
		glGetProgramiv(glHandle, GL_VALIDATE_STATUS, &isValid);

		return !isValid;
	}

	void ShaderProgram::detachShaders() {

		for (std::uint32_t shaderHandle : shaderHandles)
			glDetachShader(glHandle, shaderHandle);
	}

	otg::ShaderProgram::~ShaderProgram() {
		glDeleteProgram(glHandle);
	}

	void otg::ShaderProgram::use() const {
		glUseProgram(glHandle);
	}

}