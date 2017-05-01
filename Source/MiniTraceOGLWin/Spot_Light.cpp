#include "Spot_Light.h"
#include <math.h>

Spot_Light::Spot_Light()
{
	InitDefaultLight();
}


Spot_Light::~Spot_Light()
{
}

void Spot_Light::InitDefaultLight()
{
	SetLightPosition(0, 20, 0);
	SetLightDirection(0, -1, 0);
	SetLightColour(1, 1, 1);
	umbraAngle = 0.5;
	penumbraAngle = 0.0;

	lType = LIGHT_TYPE_Spot;
}

void Spot_Light::SetLightPosition(double x, double y, double z)
{
	m_position.SetVector(x, y, z);
}

void Spot_Light::SetLightDirection(double x, double y, double z)
{
	m_direction.SetVector(x, y, z);
	m_direction.Normalise();
}

void Spot_Light::SetLightColour(double r, double g, double b)
{
	m_colour.red = r;
	m_colour.green = g;
	m_colour.blue = b;
}

Colour Spot_Light::GetLightColour(Vector3 point)
{
	Vector3 lightVector = (point - m_position).Normalise();
	double angle = m_direction.DotProduct(lightVector);
	angle = 1 - angle;

	if ((angle <= penumbraAngle) && (angle >= 0))
	{
		return m_colour;
	}
	else if ((angle > penumbraAngle) && (angle <= umbraAngle))
	{
		return m_colour * ((angle - umbraAngle)/(penumbraAngle - umbraAngle));
	}
	return m_colour * 0;
}