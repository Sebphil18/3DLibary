#pragma once
#include <cstdint>
#include <map>
#include <string>
#include "glm/glm.hpp"

namespace glib {

	class ProgramUniformLink {

	public:
		ProgramUniformLink();
		ProgramUniformLink(std::uint32_t programHandle);

		void setUniform(const std::string& uniformName, bool value);
		void setUniform(const std::string& uniformName, int value);
		void setUniform(const std::string& uniformName, float value);

		void setUniformVec(const std::string& uniformName, glm::vec2 vec2);
		void setUniformVec(const std::string& uniformName, glm::vec3 vec3);
		void setUniformVec(const std::string& uniformName, glm::vec4 vec4);

		void setUniformMat(const std::string& uniformName, glm::mat4 mat4, bool transpose = false);

		void setProgramUniformHandle(std::uint32_t programHandle);

	private:
		std::uint32_t programHandle;
		std::map<std::string, std::int32_t> uniformLocations;

		std::int32_t getUniformLocation(const std::string& uniformName);
		bool uniformNameIsCached(const std::string& uniformName);

		void printUnusedUniform(const std::string& uniformName);

	};

}