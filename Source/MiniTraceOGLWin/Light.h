/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#pragma once

#include <math.h>
#include "Vector3.h"
#include "Material.h"

class Light
{
	protected:
		Vector3 m_position;
		Vector3 m_direction;
		Colour m_colour;

	public:		
		enum LIGHT_TYPE
		{
			LIGHT_TYPE_Point = 0,
			LIGHT_TYPE_Spot,
			LIGHT_TYPE_Directional
		};

		LIGHT_TYPE lType;

		virtual void InitDefaultLight() = 0;
		virtual void SetLightPosition(double x, double y, double z){}
		virtual void SetLightDirection(double x, double y, double z){}
		virtual void SetLightColour(double r, double g, double b) = 0;
		virtual Vector3 GetLightPosition(){ return Vector3(0, 0, 0); }
		virtual Vector3 GetPointToLightVector(Vector3 point) = 0;
		virtual Colour GetLightColour(Vector3 point) = 0;
};

