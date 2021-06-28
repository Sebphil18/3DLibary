#pragma once
#include <cstdint>
#include <string>
#include "globjects/GlObject.h"
#include "globjects/ShaderType.h"

namespace glib {

	class Shader : public  GlObject {

	public:
		Shader(const std::string& filePath, ShaderType type) noexcept;
		Shader(const Shader& other) noexcept;
		Shader(Shader&& other) noexcept;

		Shader& operator=(const Shader& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		~Shader() noexcept;

	private:
		std::uint32_t type;
		std::string src;
		std::string filePath;

		void createShader();

		void readSource();
		void loadSource();

		void tryToCompile();
		void compile();

		bool compilationFailed();

	};

}