#pragma once
#include "Light.h"

class Light_Directional : public Light
{
public:
	Light_Directional();
	~Light_Directional();

	inline void SetLightDirection(double x, double y, double z)
	{
		SetLightPosition(x, y, z);
	}

	inline Vector3 GetLightDirection()
	{
		return m_position;
	}
};

