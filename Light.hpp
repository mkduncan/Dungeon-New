#ifndef Light_H
#define Light_H

#include "Material.hpp"
#include "Shader.hpp"
#include "Vector3.hpp"

enum LightType
{
	POINT_LIGHT = 0,
	DIRECTIONAL_LIGHT = 1,
	SPOT_LIGHT = 2,
	NULL_LIGHT = 3
};

class Light
{
public:
	Vector3 Ambient, Diffuse, Specular, Position, Direction;
	float InnerCutoff, OuterCutoff, Constant, Linear, Quadradic;
	LightType Type;

	Light();
	~Light();
	Light(const Light&);
	Light(Light&&) noexcept;
	Light& operator=(const Light&);
	Light& operator=(Light&&) noexcept;

	bool Load(const Shader&, const size_t = 0) const;
};

#endif