#include "preprocessor/DefineCommand.h"

namespace glib {

	ExecutionFunction DefineCommand::get() {
		return [](const std::vector<std::string>& arguments, std::stringstream& src, std::unordered_set<std::string>& defined, std::ifstream&) {
			defined.emplace(arguments[0]);
		};
	}

}