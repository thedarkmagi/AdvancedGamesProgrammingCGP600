#include "maths.h"

maths * maths::instance = nullptr;

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

	float result = (plane.normal.x * point.x) + (plane.normal.y * point.y) + (plane.normal.z * point.z) + plane.d;
	return result;
}

ObjFileModel::xyz maths::planeIntersection(Plane * p, ObjFileModel::xyz * point1, ObjFileModel::xyz * point2)
{
	ObjFileModel::xyz ray;
	ObjFileModel::xyz pointAlongRay;
	ray.x= point1->x - point2->x;
	ray.y = point1->y - point2->y;
	ray.z = point1->z - point2->z;
	float t = (-p->d - dot(&p->normal, point1)) / dot(&p->normal, point2);

	if (t > 0.0 && t < 1.0f)
	{

		pointAlongRay.x = point1->x + (ray.x *t);
		pointAlongRay.y = point1->y + (ray.y *t);
		pointAlongRay.z = point1->z + (ray.z *t);
		return pointAlongRay;
	}
	else
	{
		pointAlongRay.x = 99999999999.0f;
		pointAlongRay.y = 99999999999.0f;
		pointAlongRay.z = 99999999999.0f;
		return pointAlongRay;
	}
}

bool maths::in_triangle(ObjFileModel::xyz * triangle0, ObjFileModel::xyz * triangle1, ObjFileModel::xyz * triangle2, ObjFileModel::xyz * point)
{
#pragma region setup vectors

	ObjFileModel::xyz AP;
	AP.x = triangle0->x - point->x;
	AP.y = triangle0->y - point->y;
	AP.z = triangle0->z - point->z;
	ObjFileModel::xyz AB;
	AB.x = triangle0->x - triangle1->x;
	AB.y = triangle0->y - triangle1->y;
	AB.z = triangle0->z - triangle1->z;

	ObjFileModel::xyz BP;
	BP.x = triangle1->x - point->x;
	BP.y = triangle1->y - point->y;
	BP.z = triangle1->z - point->z;
	ObjFileModel::xyz BC;
	BC.x =  triangle1->x- triangle2->x;
	BC.y =  triangle1->y- triangle2->y;
	BC.z =  triangle1->z- triangle2->z;

	ObjFileModel::xyz CP;
	CP.x = triangle2->x - point->x;
	CP.y = triangle2->y - point->y;
	CP.z = triangle2->z - point->z;
	ObjFileModel::xyz CA;
	CA.x = triangle2->x - triangle0->x;
	CA.y = triangle2->y - triangle0->y;
	CA.z = triangle2->z - triangle0->z;

#pragma endregion
	ObjFileModel::xyz APxAB = cross(&AP,&AB);
	ObjFileModel::xyz BPxBC = cross(&BP, &BC);
	ObjFileModel::xyz CPxCA = cross(&CP, &CA);

	float A = dot(&AP, &AB);
	float B = dot(&BP, &BC);
	float C = dot(&CP, &CA);

	if (A >= 0 && B >=0 && C >= 0)
	{
		return true;
	}
	return false;
}

int maths::sign(float number)
{
	return (number < 0.0f ? -1 : (number > 0.0f ? 1 : 0));
}
