#ifndef DungeonInterface_H
#define DungeonInterface_H

#include <chrono>

enum InterfaceType
{
	NULL_INTERFACE,
	MAIN_MENU,
	MAIN_GAME_CHOICE,
	MAIN_OPTIONS,
	MAIN_USER_GUIDE,
	GAME_USER_POPUP_MESSAGE,
	GAME_USER_POPUP_ITEM,
	GAME_USER_INVENTORY,
	GAME_CONTAINER_INVENTORY,
};

class DungeonInterface
{
private:
	DungeonInterface() = delete;
	~DungeonInterface() = delete;
	DungeonInterface(const DungeonInterface&) = delete;
	DungeonInterface(DungeonInterface&&) noexcept = delete;
	DungeonInterface& operator=(const DungeonInterface&) = delete;
	DungeonInterface& operator=(DungeonInterface&&) noexcept = delete;

	static std::chrono::time_point<std::chrono::steady_clock> Frame;
	static size_t DeltaFrame, Index;
	static InterfaceType Type;

public:
	static const bool Active();
	static const bool Open(const InterfaceType&);
	static const bool Open(const InterfaceType&, const size_t);
	static const bool Render();
	static const bool Update();
};

#endif