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

struct Colour
{
	float red;
	float green;
	float blue;
	
	Colour operator*(const Colour& other)
	{
		Colour toReturn;
		toReturn.red = other.red * this->red;
		toReturn.green = other.green * this->green;
		toReturn.blue = other.blue * this->blue;
		return toReturn;
	}

	Colour operator*(const float& other)
	{
		Colour toReturn;
		toReturn.red = other * this->red;
		toReturn.green = other * this->green;
		toReturn.blue = other * this->blue;
		return toReturn;
	}

	Colour operator/(const float& other)
	{
		Colour toReturn;
		toReturn.red = this->red / other;
		toReturn.green = this->green / other;
		toReturn.blue = this->blue / other;
		return toReturn;
	}

	void operator/=(const float& other)
	{
		this->red = this->red / other;
		this->green = this->green / other;
		this->blue = this->blue / other;
	}

	void operator+=(const Colour& other)
	{
		this->red += other.red;
		this->green += other.green;
		this->blue += other.blue;
	}

	void operator+=(const double& other)
	{
		this->red += other;
		this->green += other;
		this->blue += other;
	}

	void operator-=(const Colour& other)
	{
		this->red -= other.red;
		this->green -= other.green;
		this->blue -= other.blue;
	}

	void operator*=(const Colour& other)
	{
		this->red = other.red * this->red;
		this->green = other.green * this->green;
		this->blue = other.blue * this->blue;
	}

	bool operator!=(const Colour& other)
	{
		if (this->red != other.red)
		{
			return true;
		}
		if (this->green != other.green)
		{
			return true;
		}
		if (this->blue != other.blue)
		{
			return true;
		}
		return false;
	}

	bool operator==(const Colour& other)
	{
		if (this->red != other.red)
		{
			return false;
		}
		if (this->green != other.green)
		{
			return false;
		}
		if (this->blue != other.blue)
		{
			return false;
		}
		return true;
	}
};

class Material
{
	private:
		Colour m_ambient;
		Colour m_diffuse;
		Colour m_specular;
		double m_specpower;
		double m_refractiveIndex;
		double m_transparency;
		double m_reflectivity;
		bool m_castShadow;

	public:
		
		Material();
		~Material();

		void SetDefaultMaterial();
		void SetAmbientColour(float r, float g, float b);
		void SetDiffuseColour(float r, float g, float b);
		void SetSpecularColour(float r, float g, float b);
		void SetSpecPower(double spow);
		void SetRefractInd(double rInd);
		void SetTransparency(double transparency);
		void SetReflectivity(double reflectivity);

		inline void SetCastShadow(bool castShadow)
		{
			m_castShadow = castShadow;
		}

		inline Colour GetAmbientColour()
		{
			return m_ambient;
		}
		
		inline Colour GetDiffuseColour()
		{
			return m_diffuse;
		}

		inline Colour GetSpecularColour()
		{
			return m_specular;
		}

		inline double GetSpecPower()
		{
			return m_specpower;
		}
		
		inline double GetRefractInd()
		{
			return m_refractiveIndex;
		}

		inline double GetTransparency()
		{
			return m_transparency;
		}

		inline double GetReflectivity()
		{
			return m_reflectivity;
		}

		inline bool CastShadow()
		{
			return m_castShadow;
		}
};

