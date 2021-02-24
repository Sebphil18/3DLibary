#include "io/FileReader.h"
#include <string>
#include "gtest/gtest.h"

class FileReaderTest : public ::testing::Test {

protected:
	otg::FileReader reader;

};

TEST_F(FileReaderTest, read) {

	std::string reference = "This is a test!\nThis is line2 :)\n";
	std::string fileContent = reader.read("testFile.txt");

	ASSERT_EQ(reference, fileContent);
}