#pragma once
#include <string>
#include <functional>
#include <unordered_set>
#include <sstream>
#include <fstream>

namespace glib {

	typedef std::function<void(const std::vector<std::string>&, std::stringstream&, std::unordered_set<std::string>&, std::ifstream&)> ExecutionFunction;

	enum class CommandType {
		None, Include, Define
	};

	class CommandTypes {

	public:
		static CommandType getType(const std::string& typeStr);
		static ExecutionFunction getFunction(CommandType type);

	private:
		static bool equalsIgnoreCase(const std::string& str1, const std::string& str2);
	};

}