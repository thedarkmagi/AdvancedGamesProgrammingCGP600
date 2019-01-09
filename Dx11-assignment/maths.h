#pragma once
#include "objfilemodel.h"

class maths
{
public:
	maths();
	~maths();

	float dot(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2);
	ObjFileModel::xyz cross(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2);
	ObjFileModel::xyz normal(ObjFileModel::xyz* v1, ObjFileModel::xyz* v2, ObjFileModel::xyz* v3);

	struct Plane
	{
		ObjFileModel::xyz normal; //the normal to the plane 
		float d; //the 'd' constant in the equation for this 
	};
};

