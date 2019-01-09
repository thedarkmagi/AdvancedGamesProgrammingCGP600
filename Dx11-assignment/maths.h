#pragma once
#include "objfilemodel.h"

class maths
{
public:
	struct Plane
	{
		ObjFileModel::xyz normal; //the normal to the plane 
		float d; //the 'd' constant in the equation for this 
	};
	maths();
	~maths();

	float dot(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2);
	ObjFileModel::xyz cross(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2);
	ObjFileModel::xyz normal(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2, ObjFileModel::xyz* v3);

	Plane planeEquation(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2, ObjFileModel::xyz* v3);
	float comparePlaneToPoint(Plane plane, ObjFileModel::xyz point);
	
};

