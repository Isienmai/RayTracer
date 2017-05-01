#pragma	once
#include "Primitive.h"
#include "Vector3.h"
#include "Disc.h"

//Note: For the sake of simplicity this cylinder will always be vertical
class Cylinder : public Primitive
{
private:
	Disc m_ends[2];
	Vector3 m_position;
	double m_height, m_radius;
public:
	Cylinder();
	Cylinder(Vector3 pos, double height, double rad);
	~Cylinder();

	void SetCylinder(Vector3 pos, double height, double rad);

	RayHitResult IntersectByRay(Ray& ray);
};

