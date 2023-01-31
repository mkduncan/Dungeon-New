#ifndef DungeonDebugger_H
#define DungeonDebugger_H

#include <string>

class DungeonDebugger
{
private:
	DungeonDebugger() = delete;
	~DungeonDebugger() = delete;
	DungeonDebugger(const DungeonDebugger&) = delete;
	DungeonDebugger(DungeonDebugger&&) = delete;
	DungeonDebugger& operator=(const DungeonDebugger&) = delete;
	DungeonDebugger& operator=(DungeonDebugger&&) = delete;

	static bool IsRoomCorner(const size_t, const size_t);
	static bool IsCorridorEnd(const size_t, const size_t);
	static bool IsIsolated(const size_t, const size_t);

public:
	static bool WriteImage(const std::string& = std::string());
};

#endif