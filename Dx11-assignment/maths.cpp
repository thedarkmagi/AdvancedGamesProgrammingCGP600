#include "maths.h"



maths::maths()
{
}


maths::~maths()
{
}

float maths::dot(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}

ObjFileModel::xyz maths::cross(ObjFileModel::xyz * v1, ObjFileModel::xyz * v2)
{
	ObjFileModel::xyz v3;
	v3.x = (v1->y * v2->z) - (v1->z * v2->y);
	v3.y = (v1->z * v2->x) - (v1->x * v2->z);
	v3.z = (v1->x * v2->y) - (v1->y * v2->x);

	return v3;
}

ObjFileModel::xyz maths::normal(ObjFileModel::xyz * v1, ObjFileModel::xyz * v2, ObjFileModel::xyz * v3)
{
	ObjFileModel::xyz p01;
	ObjFileModel::xyz p02;

	p01.x = v2->x - v1->x;
	p01.y = v2->y - v1->y;
	p01.z = v2->z - v1->z;

	p02.x = v3->x - v1->x;
	p02.y = v3->y - v1->y;
	p02.z = v3->z - v1->z;
	ObjFileModel::xyz crossed = cross(&p01, &p02);
	crossed.x = crossed.x *crossed.x;
	crossed.y = crossed.y * crossed.y;
	crossed.z *= crossed.z;
	float mag = sqrt( (crossed.x * crossed.x) + (crossed.y * crossed.y) + (crossed.z * crossed.z));
	crossed.x /= mag;
	crossed.y /= mag;
	crossed.z /= mag;

	return crossed;
}

maths::Plane maths::planeEquation(ObjFileModel::xyz * v1, ObjFileModel::xyz * v2, ObjFileModel::xyz * v3)
{
	Plane planeLocal;
	planeLocal.normal = normal(v1, v2, v3);
	planeLocal.d = -dot(&planeLocal.normal, v1);
	return planeLocal;
}

float maths::comparePlaneToPoint(Plane plane, ObjFileModel::xyz point)
{

	float result = (plane.normal.x * point.x) + (plane.normal.y * point.y) + (plane.normal.z * point.z) + d;
	return result;
}
