#include "TraceManager.hpp"

#include <fstream>
#include <iostream>
#include <string>

std::queue<std::string> TraceManager::Traces = std::queue<std::string>();
static bool InitializeTraceManager = TraceManager::AddMessage(std::string());

bool TraceManager::AddMessage(const std::string& message, const size_t line, const bool returnCode)
{
	if (TRACE_ENABLED)
	{
		if (TraceManager::Traces.empty())
			TraceManager::Traces.push("Event logger has been initialized. This should be the first message.");

		if (!message.empty())
		{
			if (line == 0)
				TraceManager::Traces.push("Error: " + message + "()");

			else
				TraceManager::Traces.push("Error: " + message + "() - " + std::to_string(line));
		}
	}

	return returnCode;
}

bool TraceManager::CheckErrorGL(const std::string& message, const size_t line)
{
	if (TRACE_ENABLED)
	{
		GLenum errorCode = glGetError();
		bool result = true;

		while (errorCode != GL_NO_ERROR)
		{
			TraceManager::AddMessage(message + "() - " + std::to_string(line));
			TraceManager::AddMessage("\tError: OpenGL reported error with code " + std::to_string(errorCode));

			errorCode = glGetError();
			result = false;
		}

		return result;
	}

	return true;
}

bool TraceManager::PrintMessages(const std::string& filePath)
{
	if (TRACE_ENABLED)
	{
		std::ofstream output;
		std::string outputBuffer;

		if (filePath.empty())
		{
			while (!TraceManager::Traces.empty())
			{
				std::cout << TraceManager::Traces.front() << "\n";
				TraceManager::Traces.pop();
			}

			std::cout << "Event logger has been terminated. This should be the last message.\n" << std::endl;
			return true;
		}

		output.open(filePath.c_str());

		if (!output)
			return TraceManager::AddMessage(__FUNCTION__, __LINE__);

		while (!TraceManager::Traces.empty())
		{
			outputBuffer += TraceManager::Traces.front() + "\n";
			TraceManager::Traces.pop();
		}

		outputBuffer += "Event logger has been terminated. This should be the last message.\n";
		output << outputBuffer << std::endl;
		outputBuffer.clear();
		output.close();
	}

	return true;
}