#pragma once
#include "Light.h"

class Spot_Light : public Light 
{
	double umbraAngle, penumbraAngle;
public:
	Spot_Light();
	~Spot_Light();

	void InitDefaultLight();
	void SetLightPosition(double x, double y, double z);
	void SetLightDirection(double x, double y, double z);
	void SetLightColour(double r, double g, double b);

	void SetAngles(double umbra, double penumbra)
	{
		umbraAngle = umbra;
		penumbraAngle = penumbra;
	}

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

