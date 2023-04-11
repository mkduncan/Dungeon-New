#include "Logger.hpp"
#include "Window.hpp"

#include <fstream>
#include <iostream>
#include <string>

std::queue<std::string> Logger::Logs;
static bool InitializeLogger = Logger::LogError("", 0);

const bool Logger::LogError(const std::string& functionName, const size_t lineNumber, const bool returnCode)
{
	if (TRACE_ENABLED)
	{
		if (Logger::Logs.empty())
			Logger::Logs.push("Event logger has been initialized. This should be the first message.");

		if (!functionName.empty())
			Logger::Logs.push("Error: " + functionName + "() - " + std::to_string(lineNumber));
	}

	return returnCode;
}

const bool Logger::LogErrorGL(const std::string& functionName, const size_t lineNumber)
{
	if (TRACE_ENABLED)
	{
		GLenum errorCode = 0;
		bool result = true;

		if (Window::Interface == nullptr)
			return Logger::LogError(__FUNCTION__, __LINE__);

		errorCode = glGetError();

		if (errorCode != GL_NO_ERROR)
			Logger::Logs.push("Error: " + functionName + "() - " + std::to_string(lineNumber));

		while (errorCode != GL_NO_ERROR)
		{
			Logger::Logs.push("\tError: OpenGL reported error with code " + std::to_string(errorCode));

			errorCode = glGetError();
			result = false;
		}

		return result;
	}

	return true;
}

const bool Logger::OutputLogs(const std::string& filePath)
{
	if (TRACE_ENABLED)
	{
		std::ofstream output;
		std::string outputBuffer;

		if (filePath.empty())
		{
			while (!Logger::Logs.empty())
			{
				std::cout << Logger::Logs.front() << "\n";
				Logger::Logs.pop();
			}

			std::cout << "Event logger has been terminated. This should be the last message.\n" << std::endl;
			return true;
		}

		output.open(filePath.c_str());

		if (!output)
			return Logger::LogError(__FUNCTION__, __LINE__);

		while (!Logger::Logs.empty())
		{
			outputBuffer += Logger::Logs.front() + "\n";
			Logger::Logs.pop();
		}

		outputBuffer += "Event logger has been terminated. This should be the last message.\n";
		output << outputBuffer << std::endl;
		outputBuffer.clear();
		output.close();
	}

	return true;
}