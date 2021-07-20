#include "preprocessor/IncludeFunction.h"
#include <regex>
#include <iostream>
#include <fstream>

namespace glib {

	std::regex IncludeFunction::commandReg = std::regex("^\\[([^\\[\\]]+)\\]$");

	ExecutionFunction IncludeFunction::get() {

		return [](const std::vector<std::string>& arguments, std::stringstream& src, std::unordered_set<std::string>& defined, std::ifstream&) {

			const std::string& filePath = arguments[0];

			if (defined.find(filePath) == defined.end())
				includeFile(filePath, src, defined);
			else
				std::cout << "WARNING::SourceBuild::Tried to include " << filePath << " twice! \n";
		};
	}

	void IncludeFunction::includeFile(const std::string& filePath, std::stringstream& src, std::unordered_set<std::string>& defined) {

		std::ifstream includeFile(filePath);

		if (includeFile.good()) {
			defined.emplace(filePath);
			parseLines(includeFile, src, defined);
		} else
			std::cout << "ERROR::Include::Could not include " << filePath << "!\n";
	}

	void IncludeFunction::parseLines(std::ifstream& file, std::stringstream& src, std::unordered_set<std::string>& defined) {
		std::string line;
		while (std::getline(file, line))
			parseLine(file, line, src, defined);
	}

	void IncludeFunction::parseLine(std::ifstream& file, const std::string& line, std::stringstream& src, std::unordered_set<std::string>& defined) {

		bool isCommand = std::regex_match(line, commandReg);
		if (isCommand) {
			glib::Command cmd(line);
			cmd(src, defined, file);
		} else
			src << line << "\n";
	}

}