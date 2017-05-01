#include "Directional_Light.h"


Directional_Light::Directional_Light()
{
	InitDefaultLight();
}


Directional_Light::~Directional_Light()
{

}

void Directional_Light::InitDefaultLight()
{
	//set default direction and colour for a light
	SetLightColour(1.0, 1.0, 1.0);
	SetLightDirection(0.0, -1.0, 0.0);

	lType = LIGHT_TYPE_Directional;
}

void Directional_Light::SetLightDirection(double x, double y, double z)
{
	m_direction.SetVector(x, y, z);
	m_direction.Normalise();
}

void Directional_Light::SetLightColour(double r, double g, double b)
{
	m_colour.red = r;
	m_colour.green = g;
	m_colour.blue = b;
}