#include "globjects/ProgramUniformLink.h"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

#include "debugflags/DebugFlags.h"

#ifdef SEB_DEBUG_MODE
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
			glProgramUniform1i(programHandle, uniformLocation, value);
	}

	void ProgramUniformLink::setUniform(const std::string& uniformName, float value) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else 
			glProgramUniform1f(programHandle, uniformLocation, value);
	}

	void otg::ProgramUniformLink::setUniformVec(const std::string& uniformName, glm::vec2 vec2) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else 
			glProgramUniform2f(programHandle, uniformLocation, vec2.x, vec2.y);
		
	}

	void otg::ProgramUniformLink::setUniformVec(const std::string& uniformName, glm::vec3 vec3) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1)
			printUnusedUniform(uniformName);
		else
			glProgramUniform3f(programHandle, uniformLocation, vec3.x, vec3.y, vec3.z);
	}

	void otg::ProgramUniformLink::setUniformVec(const std::string& uniformName, glm::vec4 vec4) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1)
			printUnusedUniform(uniformName);
		else
			glProgramUniform4f(programHandle, uniformLocation, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	void otg::ProgramUniformLink::setUniformMat(const std::string& uniformName, glm::mat4 mat4, bool transpose) {

		std::int32_t uniformLocation = getUniformLocation(uniformName);

		if (uniformLocation == -1) 
			printUnusedUniform(uniformName);
		else
			glProgramUniformMatrix4fv(programHandle, uniformLocation, 1, transpose, glm::value_ptr(mat4));
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

		return itr != uniformLocations.end();
	}

	void otg::ProgramUniformLink::printUnusedUniform(const std::string& uniformName) {
#if SEB_DEBUG_MODE == 1
		std::cout << "INFO::SHADERPROGRAM::Uniform '" << uniformName << "' not found. (It might got removed because it was not used in a shader.)\n";
#endif
	}

	void otg::ProgramUniformLink::setProgramUniformHandle(std::uint32_t programHandle) {
		this->programHandle = programHandle;
	}

}