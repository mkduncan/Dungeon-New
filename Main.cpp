#include "Header.hpp"

int main(int argc, char** argv)
{
	if (!Window::Create())
	{
		Logger::OutputLogs(DEBUG_LOG);
		return 1;
	}

	else if (!DungeonScene::Initialize())
	{
		Window::Destroy();
		Logger::OutputLogs(DEBUG_LOG);

		return 2;
	}

	else if (!DungeonScene::Generate(0))
	{
		Window::Destroy();
		Logger::OutputLogs(DEBUG_LOG);

		return 3;
	}

	while (Window::Update())
	{
		if (!DungeonScene::Update())
		{
			DungeonScene::Destroy();
			Window::Destroy();
			Logger::OutputLogs(DEBUG_LOG);

			return 4;
		}

		else if (!DungeonScene::Render())
		{
			DungeonScene::Destroy();
			Window::Destroy();
			Logger::OutputLogs(DEBUG_LOG);

			return 5;
		}
	}

	DungeonScene::Destroy();
	Window::Destroy();
	Logger::OutputLogs(DEBUG_LOG);

	return 0;
}