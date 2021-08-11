#include "preprocessor/SourceBuild.h"
#include <fstream>
#include <iostream>

namespace glib {

	std::regex SourceBuild::commandRegex = std::regex("^\\[([^\\[\\]]+)\\]$");
	std::regex SourceBuild::versionRegex = std::regex("^#version\\s([0-9]+)\\s?([A-Za-z]*)$", std::regex::ECMAScript);

	SourceBuild::SourceBuild() {}

	glib::SourceBuild::SourceBuild(const char* filePath) :
		SourceBuild(std::string(filePath))
	{
	}

	SourceBuild::SourceBuild(const std::string& filePath) {
		build(filePath);
	}

	SourceBuild::operator std::string() const noexcept {
		return src.str();
	}

	void SourceBuild::build(const std::string& filePath) {
		defined.emplace(filePath);
		buildSrc(filePath);
	}

	void SourceBuild::buildSrc(const std::string& filePath) {
		file.open(filePath);
		if (file.good())
			parseLines();
		else
			std::cout << "ERROR::Include::Could not include " << filePath << "!\n";
	}

	void SourceBuild::parseLines() {
		std::string line;
		while (std::getline(file, line))
			parseLine(line);
	}

	void SourceBuild::parseLine(const std::string& line) {
		
		bool isVersion = std::regex_match(line, versionRegex);
		bool isCommand = std::regex_match(line, commandRegex);

		if (isVersion) {
			insertGlslDefines(line);
		} else if (isCommand) {
			Command cmd(line);
			cmd(src, defined, file);
		} else
			src << line << "\n";
	}

	void SourceBuild::insertGlslDefines(const std::string& versionStr) {
		// add new line (in case stream is currently at a preprocessor derective)
		src << versionStr << "\n";
		for (const auto& define : glslDefines)
			src << define << "\n";
	}

	void SourceBuild::executeCommand(const std::string& cmdStr) {
		Command cmd(cmdStr);
		cmd(src, defined, file);
	}

	void SourceBuild::addGlslDefine(const std::string& define) {
		glslDefines.insert(define);
	}

	std::string SourceBuild::getStr() const {
		return src.str();
	}

}