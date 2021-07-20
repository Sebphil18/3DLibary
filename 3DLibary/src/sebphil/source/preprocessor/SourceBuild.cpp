#include "preprocessor/SourceBuild.h"
#include <fstream>
#include <iostream>

namespace glib {

	std::regex SourceBuild::commandReg = std::regex("^\\[([^\\[\\]]+)\\]$");

	SourceBuild::SourceBuild() {}
	SourceBuild::SourceBuild(const std::string& filePath) {
		build(filePath);
	}

	SourceBuild::operator std::string() const noexcept {
		return src.str();
	}

	void SourceBuild::build(const std::string& filePath) {
		defined.emplace(filePath);
		buildFile(filePath);
	}

	void SourceBuild::buildFile(const std::string& filePath) {
		std::ifstream file(filePath);
		if (file.good())
			parseLines(file);
		else
			std::cout << "ERROR::Include::Could not include " << filePath << "!\n";
	}

	void SourceBuild::parseLines(std::ifstream& file) {
		std::string line;
		while (std::getline(file, line))
			parseLine(line, file);
	}

	void SourceBuild::parseLine(const std::string& line, std::ifstream& file) {

		bool isCommand = std::regex_match(line, commandReg);

		if (isCommand) {
			glib::Command cmd(line);
			cmd(src, defined, file);
		} else
			src << line << "\n";
	}

	std::string SourceBuild::getStr() const {
		return src.str();
	}

}