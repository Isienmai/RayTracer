#pragma once
#include "Primitive.h"
#include "Vector3.h"
#include "Ray.h"
#include <vector>

class Disc : public Primitive
{
private:
	Vector3 m_centre;
	Vector3 m_normal;
	double m_radius;
public:
	Disc();
	Disc(Vector3 centre, Vector3 normal, double radius);
	~Disc();

	void SetDisc(Vector3 centre, Vector3 normal, double radius);

	RayHitResult IntersectByRay(Ray& ray);
};

