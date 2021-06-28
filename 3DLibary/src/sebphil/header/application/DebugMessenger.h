#pragma once
#include <functional>
#include <string>
#include "glad/glad.h"

// This is not nice to read but this signature is worse
#define SEB_GL_MSG_CALLBACK_FUNC_PTR void (*glMessageCallback)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)

namespace glib {

	struct Message {
		std::uint32_t id;
		std::string message;
		std::string type;
		std::string severity;
		std::string source;
	};

	class DebugMessenger {

	public:
		DebugMessenger();
		DebugMessenger(SEB_GL_MSG_CALLBACK_FUNC_PTR);

		void setGlMessageCallback(SEB_GL_MSG_CALLBACK_FUNC_PTR);

	private:
		SEB_GL_MSG_CALLBACK_FUNC_PTR;

		static void standardMsgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
		static void printMessage(const Message& msg);

		static std::string getSourceStr(std::uint32_t source);
		static std::string getTypeStr(std::uint32_t type);
		static std::string getSeverityStr(std::uint32_t severity);

	};

}