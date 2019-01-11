#pragma once
#pragma once
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <stdio.h>

#include "objfilemodel.h"

class maths
{
public:

	static maths* instance;
	static maths* getInstance()
	{
		if (!instance)
		{
			instance = new maths();
		}
		return instance;
	}
	maths(maths const&) = delete;
	void operator=(maths const&) = delete;


	struct Plane
	{
		ObjFileModel::xyz normal; //the normal to the plane 
		float d; //the 'd' constant in the equation for this 
	};
	

	float dot(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2);
	ObjFileModel::xyz cross(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2);
	ObjFileModel::xyz normal(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2, ObjFileModel::xyz* v3);

	Plane planeEquation(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2, ObjFileModel::xyz* v3);
	float comparePlaneToPoint(Plane plane, ObjFileModel::xyz point);
	ObjFileModel::xyz planeIntersection(Plane* p, ObjFileModel::xyz* point1, ObjFileModel::xyz* point2);
	bool in_triangle(ObjFileModel::xyz* triangle0, ObjFileModel::xyz* triangle1, ObjFileModel::xyz* triangle2, ObjFileModel::xyz* point);
	int sign(float number);
private:
	maths();
	~maths();
};

