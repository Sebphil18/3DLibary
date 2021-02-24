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
		glHandle = glCreateShader(this->type);

		readSource();
		loadSource();

		tryToCompile();
	}

	void Shader::readSource() {

		otg::FileReader fileReader;
		
		src = fileReader.read(filePath);
	}

	void Shader::loadSource() {

		const char* srcPtr = src.c_str();
		glShaderSource(glHandle, 1, &srcPtr, NULL);
	}

	Shader::Shader(const Shader& other) noexcept :
		filePath(other.filePath), src(other.src), type(other.type) {

		glHandle = glCreateShader(type);

		tryToCompile();
	}

	Shader& Shader::operator=(const Shader& other) noexcept {

		filePath = other.filePath;
		src = other.src;
		type = other.type;

		glHandle = glCreateShader(type);

		tryToCompile();

		return *this;
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
		filePath(std::move(other.filePath)), 
		src(std::move(other.src)), 
		type(std::move(other.type)), 
		glHandle(std::move(other.glHandle)) 
	{}

	Shader& Shader::operator=(Shader&& other) noexcept {

		filePath = std::move(other.filePath);
		src = std::move(other.src);
		type = std::move(other.type);
		glHandle = std::move(other.glHandle);

		return *this;
	}

	Shader::~Shader() noexcept {
		glDeleteShader(glHandle);
	}

	std::uint32_t otg::Shader::getGlHandle() const {
		return glHandle;
	}
}