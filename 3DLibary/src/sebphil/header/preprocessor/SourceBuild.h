#pragma once
#include <string>
#include "Command.h"

namespace glib {

	class SourceBuild {

	public:
		SourceBuild();
		SourceBuild(const std::string& filePath);

		operator std::string() const noexcept;

		void build(const std::string& filePath);

		std::string getStr() const;

	private:
		std::stringstream src;
		std::unordered_set<std::string> defined;
		std::vector<Command> commands;
		static std::regex commandReg;

		void buildFile(const std::string& filePath);
		void parseLines(std::ifstream& file);
		void parseLine(const std::string& line, std::ifstream& file);

	};

}