#include "Point_Light.h"


Point_Light::Point_Light()
{
	InitDefaultLight();
}


Point_Light::~Point_Light()
{
}

void Point_Light::InitDefaultLight()
{
	//set default position and colour for a light
	SetLightColour(1.0, 1.0, 1.0);
	SetLightPosition(0.0, 20.0, 0.0);

	lType = LIGHT_TYPE_Point;
}

void Point_Light::SetLightPosition(double x, double y, double z)
{
	m_position.SetVector(x, y, z);
}

Colour Point_Light::GetLightColour(Vector3 point)
{
	//return m_colour;
	double dist = (point - m_position).Norm();

	return m_colour * (10 / dist);
}

void Point_Light::SetLightColour(double r, double g, double b)
{
	m_colour.red = r;
	m_colour.green = g;
	m_colour.blue = b;
}


