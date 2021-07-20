#pragma once
#include <vector>
#include <regex>
#include <functional>
#include <sstream>
#include <unordered_set>
#include "CommandTypes.h"

namespace glib {

	class Command {

	public:

		Command(const std::string& cmd);

		CommandType getType() const;
		std::string getArgument(std::size_t index) const;
		std::string getStr() const;

		void operator()(std::stringstream& src, std::unordered_set<std::string>& defined, std::ifstream& file) noexcept;

	private:
		std::string cmdStr;
		CommandType type;
		std::vector<std::string> arguments;
		ExecutionFunction execute;

		CommandType getType(std::sregex_iterator& token);
		std::vector<std::string> getArguments(std::sregex_iterator& token);

	};
}