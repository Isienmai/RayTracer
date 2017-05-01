#include <math.h>
#include "Cylinder.h"

Cylinder::Cylinder()
{
	SetCylinder(Vector3(0.0, 0.0, 0.0), 1, 1);
	m_primtype = Primitive::PRIMTYPE_Cylinder;
}

Cylinder::Cylinder(Vector3 pos, double height, double rad)
{
	SetCylinder(pos, height, rad);
	m_primtype = Primitive::PRIMTYPE_Cylinder;
}


Cylinder::~Cylinder()
{
}

void Cylinder::SetCylinder(Vector3 pos, double height, double rad)
{	
	m_position = pos;
	m_height = height;
	m_radius = rad;

	m_ends[0].SetDisc(Vector3(m_position[0], m_position[1] + (m_height / 2.0), m_position[2]), Vector3(0.0, 1.0, 0.0), m_radius);
	m_ends[1].SetDisc(Vector3(m_position[0], m_position[1] - (m_height / 2.0), m_position[2]), Vector3(0.0, -1.0, 0.0), m_radius);
}

RayHitResult Cylinder::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;

	bool planeCollision = false;

	double t = FARFAR_AWAY;
	Vector3 normal;
	Vector3 intersection_point;


	//Did the ray collide with the top or bottom of the cylinder from outside the cylinder?


	for (int i = 0; i < 2; i++)
	{
		Primitive* prim = static_cast<Primitive*>(&m_ends[i]);

		RayHitResult tempresult = prim->IntersectByRay(ray);

		if (tempresult.t < result.t)
		{
			tempresult.data = this;
			result = tempresult;
			return result;
		}
	}


	//If not, did the ray collide with the cylinder(ignoring any vertical component)?


	//treat the entire scene as 2d, viewed from top down.
	Vector3 m_centre = m_position;
	m_centre[1] = 0;
	Vector3 topDownRay = ray.GetRay();
	topDownRay[1] = 0;
	Vector3 dOrigin = ray.GetRayStart();
	dOrigin[1] = 0;

	////perform repeated calculations once
	//get a ray from the origin of the given ray to the centre of the sphere
	Vector3 relativeRay = dOrigin - m_centre;
	//get the length of the provided ray squared
	double raySelfDot = topDownRay.DotProduct(topDownRay);

	//find t
	t = pow(topDownRay.DotProduct(relativeRay), 2);
	t -= raySelfDot*(relativeRay.DotProduct(relativeRay) - pow(m_radius, 2));
	t = sqrt(t);

	//only finish the equation if there is at least one root
	if (t != 0)
	{
		//only bother with the negative root by subtracting
		t = (-1 * ray.GetRay().DotProduct(relativeRay) - t) / raySelfDot;
	}
	else
	{
		t = FARFAR_AWAY;
	}

	//no valid collision detected? return nothing
	if (t <= 0.0 || t >= FARFAR_AWAY)
	{
		return result;
	}


	//Valid cylinder collision detected. Now to re-insert the vertical component


	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;

	//get the normal
	normal = intersection_point - m_centre;
	normal[1] = 0;
	normal.Normalise();

	//##error not below here
	if ((t > 0.0 && t != FARFAR_AWAY) && (intersection_point[1] <= m_position[1] + (m_height / 2)) && (intersection_point[1] >= m_position[1] - (m_height / 2)))
	{
		result.t = t;
		result.data = this;
		result.point = intersection_point;
		result.normal = normal;
	}

	return result;
}