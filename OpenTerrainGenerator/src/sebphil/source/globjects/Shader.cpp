#include "globjects/Shader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "glad/glad.h"
#include "io/FileReader.h"
#include "exceptions/ShaderCompilationException.h"

namespace otg {

	Shader::Shader(const std::string& filePath, ShaderType type) noexcept :
		filePath(filePath), type(ShaderStageType::getGlType(type))
	{
		readSource();
		createShader();
	}

	void Shader::readSource() {

		otg::FileReader fileReader;
		
		src = fileReader.read(filePath);
	}

	Shader::Shader(const Shader& other) noexcept :
		filePath(other.filePath), src(other.src), type(other.type) {

		createShader();
	}

	Shader& Shader::operator=(const Shader& other) noexcept {

		filePath = other.filePath;
		src = other.src;
		type = other.type;

		createShader();

		return *this;
	}

	void otg::Shader::createShader() {

		glHandle = glCreateShader(this->type);

		loadSource();
		tryToCompile();
	}

	void Shader::loadSource() {

		const char* srcPtr = src.c_str();
		glShaderSource(glHandle, 1, &srcPtr, NULL);
	}

	void Shader::tryToCompile() {

		try {

			compile();

		} catch (const ShaderCompilationException& exception) {

			std::cout << exception.what() << "\n";
		}
	}

	void Shader::compile() {

		glCompileShader(glHandle);

		if (compilationFailed())
			throw ShaderCompilationException(glHandle, filePath);
	}

	bool Shader::compilationFailed() {

		std::int32_t success;
		glGetShaderiv(glHandle, GL_COMPILE_STATUS, &success);

		return !success;
	}

	Shader::Shader(Shader&& other) noexcept :
		GlObject(std::move(other)),
		filePath(std::move(other.filePath)),
		src(std::move(other.src)),
		type(std::move(other.type)) 
	{
	}

	Shader& Shader::operator=(Shader&& other) noexcept {

		filePath = std::move(other.filePath);
		src = std::move(other.src);
		type = std::move(other.type);

		glHandle = std::move(other.glHandle);
		other.glHandle = 0;

		return *this;
	}

	Shader::~Shader() noexcept {
		glDeleteShader(glHandle);
	}
}