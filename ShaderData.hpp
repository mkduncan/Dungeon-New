#ifndef ShaderData_H
#define ShaderData_H

#include <string>

class ShaderData
{
private:
	static std::string VertexColoredSimple, VertexTexturedSimple, VertexTexturedLights, FragmentColoredSimple, FragmentTexturedSimple, FragmentTexturedLights;

	ShaderData() = delete;
	~ShaderData() = delete;
	ShaderData(const ShaderData&) = delete;
	ShaderData(ShaderData&&) noexcept = delete;
	ShaderData& operator=(const ShaderData&) = delete;
	ShaderData& operator=(ShaderData&&) noexcept = delete;

public:
	static std::string VertexColored();
	static std::string VertexTextured(const bool = true);
	static std::string FragmentColored();
	static std::string FragmentTextured(const bool = true);
	static bool DefineMaxLights(const size_t, const size_t, const size_t);
};


#endif