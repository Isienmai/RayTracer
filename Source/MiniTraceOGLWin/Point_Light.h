#pragma once
#include "Light.h"
class Point_Light : public Light
{
public:
	Point_Light();
	~Point_Light();

	void InitDefaultLight();
	void SetLightPosition(double x, double y, double z);
	void SetLightColour(double r, double g, double b);

	inline Vector3 GetLightPosition()
	{
		return m_position;
	}
	inline Vector3 GetPointToLightVector(Vector3 point)
	{
		return m_position - point;
	}

	Colour GetLightColour(Vector3 point);
};

