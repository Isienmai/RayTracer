#include "Disc.h"


Disc::Disc()
{
	SetDisc(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 1.0, 0.0), 1.0);
}

Disc::Disc(Vector3 centre, Vector3 normal, double radius)
{
	SetDisc(centre, normal, radius);
}

Disc::~Disc()
{
}

void Disc::SetDisc(Vector3 centre, Vector3 normal, double radius)
{
	m_centre = centre;
	m_normal = normal;
	m_radius = radius;
	m_primtype = PRIMTYPE_Disc;
}

RayHitResult Disc::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	double t = FARFAR_AWAY;
	Vector3 intersection_point;

	double m_offset = -1 * (m_centre.DotProduct(m_normal));

	//checks if the the ray is opposing the plane's normal and calculates the colour if so
	if (ray.GetRay().DotProduct(m_normal) < 0)
	{
		t = -1 * (ray.GetRayStart().DotProduct(m_normal) + m_offset) / ray.GetRay().DotProduct(m_normal);
	}

	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;

	//if the collision point is not within the disc then ignore it
	if ((intersection_point - m_centre).Norm() > m_radius)
	{
		t = FARFAR_AWAY;
	}

	if (t > 0 && t < FARFAR_AWAY)
	{ //ray intersection
		result.t = t;
		result.normal = this->m_normal;
		result.point = intersection_point;
		result.data = this;
	}

	return result;
}

