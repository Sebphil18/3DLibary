#include "globjects/ProgramUniformLink.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "debugflags/DebugFlags.h"

#if SEB_DEBUG_MODE 1
#include <iostream>
#endif

namespace otg {

	otg::ProgramUniformLink::ProgramUniformLink():
		programHandle(0)
	{
	}

	ProgramUniformLink::ProgramUniformLink(std::uint32_t programHandle) :
		programHandle(programHandle)
	{}

	void ProgramUniformLink::setUniform(const std::string& uniformName, bool value) {
		setUniform(uniformName, (int)value);
	}

	void ProgramUniformLink::setUniform(const std::string& uniformName, int value) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else 
			glUniform1i(uniformLocation, value);
	}

	void ProgramUniformLink::setUniform(const std::string& uniformName, float value) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else 
			glUniform1f(uniformLocation, value);
	}

	void otg::ProgramUniformLink::setUniformVec(const std::string& uniformName, glm::vec2 vec2) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else 
			glUniform2f(uniformLocation, vec2.x, vec2.y);
		
	}

	void otg::ProgramUniformLink::setUniformVec(const std::string& uniformName, glm::vec3 vec3) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1)
			printUnusedUniform(uniformName);
		else
			glUniform3f(uniformLocation, vec3.x, vec3.y, vec3.z);
	}

	void otg::ProgramUniformLink::setUniformVec(const std::string& uniformName, glm::vec4 vec4) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1)
			printUnusedUniform(uniformName);
		else
			glUniform4f(uniformLocation, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	void otg::ProgramUniformLink::setUniformMat(const std::string& uniformName, glm::mat4 mat4, bool transpose) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else 
			glUniformMatrix4fv(uniformLocation, 1, transpose, glm::value_ptr(mat4));
	}

	std::int32_t otg::ProgramUniformLink::getUniformLocation(const std::string& uniformName) {

		std::int32_t uniformLocation;
		
		if (uniformNameIsCached(uniformName)) {
			uniformLocation = uniformLocations[uniformName];
		} else {
			uniformLocation = glGetUniformLocation(programHandle, uniformName.c_str());
			uniformLocations[uniformName] = uniformLocation;
		}

		return uniformLocation;
	}

	bool otg::ProgramUniformLink::uniformNameIsCached(const std::string& uniformName) {

		auto itr = uniformLocations.find(uniformName);

		if (itr == uniformLocations.end())
			return false;
		else
			return true;
	}

	void otg::ProgramUniformLink::printUnusedUniform(const std::string& uniformName) {
#if SEB_DEBUG_MODE 1
		std::cout << "WARNING::SHADERPROGRAM::Uniform '" << uniformName << "' not found. (It might got removed because it was not used in a shader.)";
#endif
	}

	void otg::ProgramUniformLink::setProgramUniformHandle(std::uint32_t programHandle) {
		this->programHandle = programHandle;
	}

}