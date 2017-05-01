/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include <math.h>
#include "Vector3.h"

double Vector3::operator [] (const int i) const
{
	return m_element[i];
}

double& Vector3::operator [] (const int i)
{
	return m_element[i];
}

Vector3 Vector3::operator + (const Vector3& rhs) const
{
	return Vector3(
		(*this)[0] + rhs[0],
		(*this)[1] + rhs[1],
		(*this)[2] + rhs[2]);
}

Vector3 Vector3::operator - (const Vector3& rhs) const
{
	return Vector3(
		(*this)[0] - rhs[0],
		(*this)[1] - rhs[1],
		(*this)[2] - rhs[2]);
}

Vector3& Vector3::operator = (const Vector3& rhs)
{
	m_element[0] = rhs[0];
	m_element[1] = rhs[1];
	m_element[2] = rhs[2];
	return *this;
}

Vector3 Vector3::operator * (const Vector3& rhs) const
{
	return Vector3(
		m_element[0] * rhs[0],
		m_element[1] * rhs[1],
		m_element[2] * rhs[2]);
}

Vector3 Vector3::operator * (double scale) const
{
	return Vector3(
		m_element[0] * scale,
		m_element[1] * scale,
		m_element[2] * scale
		);
}

double Vector3::Norm() const
{
	return sqrtf(m_element[0] * m_element[0] + m_element[1] * m_element[1] + m_element[2] * m_element[2]);
}

double Vector3::Norm_Sqr() const
{
	return m_element[0] * m_element[0] + m_element[1] * m_element[1] + m_element[2] * m_element[2];
}

double Vector3::DotProduct(const Vector3& rhs) const
{
	return m_element[0] * rhs[0] + m_element[1] * rhs[1] + m_element[2] * rhs[2];
}

Vector3 Vector3::Normalise()
{
	double length = this->Norm();

	if (length > 1.0e-8f)
	{
		double invLen = 1.0f / length;

		m_element[0] *= invLen;
		m_element[1] *= invLen;
		m_element[2] *= invLen;
	}

	return *this;
}

Vector3 Vector3::CrossProduct(const Vector3& rhs) const
{
	return Vector3(
		(m_element[1] * rhs[2] - m_element[2] * rhs[1]),
		(m_element[2] * rhs[0] - m_element[0] * rhs[2]),
		(m_element[0] * rhs[1] - m_element[1] * rhs[0])
		);
}

Vector3 Vector3::Reflect(const Vector3& n) const
{
	Vector3 result;
	
	//TODO: Calculate the reflection of this vector given the input normal n	
	//Store the result in result
	result = *this - (n * (this->DotProduct(n)) * 2);
	
	return result;
}

Vector3 Vector3::Refract(const Vector3& n, double r_coeff) const
{
	Vector3 result, originalVector = *this;
	float cosAngle = n.DotProduct(originalVector.Normalise());
	float angleFraction = 1 / r_coeff;
	if (cosAngle == 0)
	{
		return n;
	}
	
	float powedSin = pow(angleFraction, 2) * (1 - pow(cosAngle, 2));
	

	//TODO: Calculate the refraction of this vector given the input normal n and the refractive coefficient r_index
	//Store the result in result
	//equation from http://www.flipcode.com/archives/reflection_transmission.pdf
	result = (originalVector * angleFraction) - (n * (angleFraction * cosAngle + sqrt(1 - powedSin)));
	//Refraction is governed by the Snell's law
	
	return result;
}

void Vector3::SetZero()
{
	m_element[0] = m_element[1] = m_element[2] = 0.0f;
}
