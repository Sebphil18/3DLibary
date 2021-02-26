#pragma once
#include <cstdint>
#include <exception>
#include <iostream>

namespace otg {

	class ProgramBuildException : public std::exception {

	public:
		ProgramBuildException(std::uint32_t programHandle);

		const char* what() const override;

	private:
		std::uint32_t programHandle;
		std::string message;

		std::string getLog();
		std::int32_t getLogLength();

		void constructMessage();

	};

}
