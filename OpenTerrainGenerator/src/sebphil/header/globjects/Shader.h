#pragma once
#include <cstdint>
#include <string>
#include "globjects/ShaderType.h"

namespace otg {

	class Shader {

	public:
		Shader(const std::string& filePath, ShaderType type) noexcept;
		Shader(const Shader& other) noexcept;
		Shader(Shader&& other) noexcept;

		Shader& operator=(const Shader& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		// TODO: When compilation failed the shader gets deleted twice: once by the exception and once this shader gets destructed
		~Shader() noexcept;

		std::uint32_t getGlHandle() const;

	private:
		std::uint32_t glHandle;
		std::uint32_t type;
		std::string src;
		std::string filePath;

		void readSource();
		void loadSource();

		void tryToCompile();
		void compile();

		bool compilationFailed();

	};

}