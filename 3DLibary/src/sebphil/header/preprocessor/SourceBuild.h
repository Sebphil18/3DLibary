#pragma once
#include <string>
#include "Command.h"

namespace glib {

	class SourceBuild {

	public:
		SourceBuild();
		SourceBuild(const char* filePath);
		SourceBuild(const std::string& filePath);

		operator std::string() const noexcept;

		void build(const std::string& filePath);
		void addGlslDefine(const std::string& define);

		std::string getStr() const;

	private:
		std::stringstream src;
		std::unordered_set<std::string> defined, glslDefines;
		std::vector<Command> commands;
		std::ifstream file;
		static std::regex commandRegex, versionRegex;

		void buildSrc(const std::string& filePath);
		void parseLines();
		void parseLine(const std::string& line);
		void insertGlslDefines(const std::string& versionStr);
		void executeCommand(const std::string& cmdStr);

	};

}