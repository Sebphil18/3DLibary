#pragma once
#include "CommandTypes.h"
#include "Command.h"

namespace glib {

	class IncludeFunction {

	public:
		static ExecutionFunction get();

	private:
		static std::regex commandReg;

		static void includeFile(const std::string& filePath, std::stringstream& src, std::unordered_set<std::string>& defined);
		static void parseLines(std::ifstream& file, std::stringstream& src, std::unordered_set<std::string>& defined);
		static void parseLine(std::ifstream& file, const std::string& line, std::stringstream& src, std::unordered_set<std::string>& defined);

	};

}