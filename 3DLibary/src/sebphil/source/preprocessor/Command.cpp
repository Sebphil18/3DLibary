#include "preprocessor/Command.h"
#include <iostream>

namespace glib {
	
	Command::Command(const std::string& cmd) : 
		cmdStr(cmd)
	{
		std::regex tokenizer("(\\w+)(\\w|[^\\]\\[\\s])+");
		auto token = std::sregex_iterator(cmd.begin(), cmd.end(), tokenizer);

		type = getType(token);
		execute = CommandTypes::getFunction(type);
		arguments = getArguments(token);
	}

	void Command::operator()(std::stringstream& src, std::unordered_set<std::string>& defined, std::ifstream& file) noexcept {
		execute(arguments, src, defined, file);
	}

	CommandType Command::getType(std::sregex_iterator& token) {
		CommandType type = CommandTypes::getType(token->str());
		token++;
		return type;
	}

	std::vector<std::string> Command::getArguments(std::sregex_iterator& token) {
		auto end = std::sregex_iterator();
		std::vector<std::string> arguments;
		for (; token != end; token++)
			arguments.push_back(token->str());
		return arguments;
	}

	CommandType Command::getType() const {
		return type;
	}

	std::string Command::getArgument(std::size_t index) const {
		return arguments[index];
	}

	std::string Command::getStr() const {
		return cmdStr;
	}

}