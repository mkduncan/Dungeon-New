#ifndef TraceManager_H
#define TraceManager_H

#ifndef NDEBUG
#define NDEBUG 0
#endif
#ifdef TRACE_ENABLED
#undef TRACE_ENABLED
#endif
#define TRACE_ENABLED 1//!NDEBUG

#include <glad/glad.h>
#include <queue>
#include <string>

class TraceManager
{
private:
	static std::queue<std::string> Traces;

	TraceManager() = delete;
	~TraceManager() = delete;
	TraceManager(const TraceManager&) = delete;
	TraceManager(TraceManager&&) noexcept = delete;
	TraceManager& operator=(const TraceManager&) = delete;
	TraceManager& operator=(TraceManager&&) noexcept = delete;

public:
	static bool AddMessage(const std::string&, const size_t = 0, const bool = false);
	static bool CheckErrorGL(const std::string&, const size_t = 0);
	static bool PrintMessages(const std::string & = std::string());
};

#endif