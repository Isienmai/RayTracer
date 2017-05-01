#pragma once
#include "Light.h"

class Directional_Light : public Light
{
public:
	Directional_Light();
	~Directional_Light();
	
	void InitDefaultLight();
	void SetLightDirection(double x, double y, double z);
	void SetLightColour(double r, double g, double b);

	Vector3 GetPointToLightVector(Vector3 point)
	{
		return m_direction * -10;
	}

	Colour GetLightColour(Vector3 point)
	{
		return m_colour;
	}
};

