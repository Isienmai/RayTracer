/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
* 
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "Triangle.h"

Triangle::Triangle()
{
	m_vertices[0] = Vector3(-1.0, 0.0, -5.0);
	m_vertices[1] = Vector3(0.0, 1.0, -5.0);
	m_vertices[2] = Vector3(1.0, 0.0, -5.0);
	m_normal = Vector3(0.0, 0.0, 1.0);
	m_primtype = PRIMTYPE_Triangle;
}

Triangle::Triangle(Vector3 pos1, Vector3 pos2, Vector3 pos3)
{
	SetTriangle(pos1, pos2, pos3);

	m_primtype = PRIMTYPE_Triangle;
}


Triangle::~Triangle()
{
}

void Triangle::SetTriangle(Vector3 v0, Vector3 v1, Vector3 v2)
{
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;

	//Calculate Normal
	Vector3 NormalA = m_vertices[1] - m_vertices[0];
	Vector3 NormalB = m_vertices[2] - m_vertices[0];
	Vector3 Norm = NormalA.CrossProduct(NormalB);
	Norm.Normalise();
	m_normal = Norm;
}


RayHitResult Triangle::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	double t = FARFAR_AWAY;
	Vector3 intersection_point;

	double m_offset = -1*(m_vertices[0].DotProduct(m_normal));

	if (ray.GetRay().DotProduct(m_normal) < 0)
	{
		t = -1 * (ray.GetRayStart().DotProduct(m_normal) + m_offset) / ray.GetRay().DotProduct(m_normal);
	}
	
	//Calculate the exact location of the intersection using the result of t
	intersection_point = ray.GetRayStart() + ray.GetRay()*t;

	//calculate edges        
	Vector3 edge1 = m_vertices[2] - m_vertices[0], edge2 = m_vertices[1] - m_vertices[0], edgeToIntersec = intersection_point - m_vertices[0];

	//calculate dot products
	double dot11 = edge1.DotProduct(edge1), dot12 = edge1.DotProduct(edge2), dot1P = edge1.DotProduct(edgeToIntersec);
	double dot22 = edge2.DotProduct(edge2), dot2P = edge2.DotProduct(edgeToIntersec);

	//calculate barycentric coordinates
	double denom = 1 / (dot11 * dot22 - dot12 * dot12);
	double u = (dot22 * dot1P - dot12 * dot2P) * denom;
	double v = (dot11 * dot2P - dot12 * dot1P) * denom;

	if ((u<0) || (v<0) || (u+v>=1))
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
