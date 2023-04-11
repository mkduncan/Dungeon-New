#ifndef Logger_H
#define Logger_H

#ifndef NDEBUG
#define NDEBUG 0
#endif
#ifdef TRACE_ENABLED
#undef TRACE_ENABLED
#endif
#define TRACE_ENABLED !NDEBUG
#define DEBUG_LOG "output.log"

#include <glad/glad.h>
#include <queue>
#include <string>

class Logger
{
private:
	static std::queue<std::string> Logs;

public:
	Logger() = delete;
	~Logger() = delete;
	Logger(const Logger&) = delete;
	Logger(Logger&&) noexcept = delete;
	Logger& operator=(const Logger&) = delete;
	Logger& operator=(Logger&&) noexcept = delete;

	static const bool LogError(const std::string&, const size_t = 0, const bool = false);
	static const bool LogErrorGL(const std::string&, const size_t = 0);
	static const bool OutputLogs(const std::string & = std::string());
};

#endif