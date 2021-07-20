#include "preprocessor/CommandTypes.h"
#include "preprocessor/IncludeFunction.h"
#include "preprocessor/DefineCommand.h"

namespace glib {

	CommandType CommandTypes::getType(const std::string& typeStr) {

		CommandType type = CommandType::None;

		if (equalsIgnoreCase(typeStr, "include"))
			type = CommandType::Include;
		else if (equalsIgnoreCase(typeStr, "define"))
			type = CommandType::Define;

		return type;
	}

	bool CommandTypes::equalsIgnoreCase(const std::string& str1, const std::string& str2) {
		return std::equal(
			str1.begin(), str1.end(), 
			str2.begin(), str2.end(), 
			[](const auto& c1, const auto& c2) {
				return std::tolower(c1) == std::tolower(c2);
			});
	}

	ExecutionFunction CommandTypes::getFunction(CommandType type) {

		ExecutionFunction func;

		switch (type) {

		case CommandType::Include:
			func = IncludeFunction::get();
			break;
		case CommandType::Define:
			func = DefineCommand::get();
			break;
		default:
			func = [](const std::vector<std::string>&, std::stringstream& src, std::unordered_set<std::string>&, std::ifstream&) {};
		}

		return func;
	}

}