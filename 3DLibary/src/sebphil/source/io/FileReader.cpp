#include "io/FileReader.h"
#include <iostream>
#include "exceptions/FileInputException.h"

namespace glib {

	FileReader::FileReader() {
	}

	std::string FileReader::read(const std::string& filePath) {

		std::string content;

		try {

			content = getFileContent(filePath);

		} catch (FileInputException& exception) {
			std::cout << exception.what() << "\n";
		}

		return content;
	}

	std::string glib::FileReader::getFileContent(const std::string& filePath) {
		
		std::ifstream fileInput(filePath);
		std::string content("");

		if (fileInput.is_open()) {

			content = writeFileToString(fileInput);

		} else
			throw FileInputException(filePath);

		fileInput.close();

		return content;
	}

	std::string glib::FileReader::writeFileToString(std::ifstream& fileInput) {

		std::string content;
		std::string line;

		while (std::getline(fileInput, line))
			content.append(line + "\n");

		return content;
	}

}