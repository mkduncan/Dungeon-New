#include "Header.hpp"
#include "DungeonDebugger.hpp"

int main(int argc, char** argv)
{
	if (!FrameRenderable::Create())
	{
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
		TraceManager::PrintMessages();

		return 1;
	}

	else if (!DungeonManager::Create())
	{
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
		TraceManager::PrintMessages();

		return 2;
	}

	DungeonDebugger::WriteImage("output.bmp");

	if (!DungeonManager::Update())
	{
		TraceManager::AddMessage(__FUNCTION__, __LINE__);
		TraceManager::PrintMessages();

		return 3;
	}

	AlignedBox test;
	OrientedBox box;

	test.Envelope(Vector3(Camera::Position[0] - 1.0f, Camera::Position[1] - 1.0f, Camera::Position[2] - 1));
	test.Envelope(Vector3(Camera::Position[0] + 1.0f, Camera::Position[1] + 1.0f, Camera::Position[2] + 1));

	float alpha = 0.0f;
	float beta = 0.0f;
	float gamma = 0.0f;

	WireFrameRenderable frame;
	std::array<Vector3, 8> corners = test.Corners();

	for (Vector3 v : corners)
		frame.AddVertex(VertexColored(Vector4(0.0f, 1.0f, 0.0f, 1.0f), v));

	frame.LoadShaders(ShaderData::VertexColored(), ShaderData::FragmentColored());
	frame.Upload();

	while (FrameRenderable::Update())
	{
		if (!DungeonManager::Update())
		{
			TraceManager::AddMessage(__FUNCTION__, __LINE__);
			TraceManager::PrintMessages();

			return 4;
		}

		else if (!DungeonManager::Render())
		{
			TraceManager::AddMessage(__FUNCTION__, __LINE__);
			TraceManager::PrintMessages();

			return 5;
		}

		box.SetBounds(test, Matrix4::Rotated(Vector3(alpha, beta, gamma)));
		box.Render();

		alpha += MathManager::Radians(10.0f) * MathManager::DeltaTime();
		beta += MathManager::Radians(-7.75f) * MathManager::DeltaTime();
		gamma += MathManager::Radians(45.0f) * MathManager::DeltaTime();

		frame.Render();
	}

	frame.Destroy();

	if (!DungeonManager::Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);

	if (!FrameRenderable::Destroy())
		TraceManager::AddMessage(__FUNCTION__, __LINE__);

	TraceManager::PrintMessages();
	return 0;
}