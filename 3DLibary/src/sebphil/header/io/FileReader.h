#pragma once
#include <fstream>
#include <string>

namespace glib {

	class FileReader {

	public:
		FileReader();

		std::string read(const std::string& filePath);

	private:
		std::string getFileContent(const std::string& filePath);
		std::string writeFileToString(std::ifstream& fileInput);

	};

}