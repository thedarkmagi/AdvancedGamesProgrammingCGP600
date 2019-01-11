#pragma once
#include <d3d11.h>
#include <math.h>
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>
#include "GameTimer.h"
class camera
{
public:
	camera(float x, float y, float z, float cameraRotation);
	~camera();
	void rotate(float nDegrees);
	void forward(float distance);
	void up();
	void strafe(float distance);
	void moveUp(float distance);
	void rotateInX(float distance);
	XMMATRIX GetViewMatix();
	XMVECTOR GetCameraPos();
	float getX();
	float getY();
	float getZ();

	float getDX();
	float getDY();
	float getDZ();
private:
	float m_x;
	float m_y;
	float m_z;
	float m_dx;
	float m_dz;
	float m_dy;
	float m_camera_rotation;
	float m_camera_rotationX;

	XMVECTOR m_position;
	XMVECTOR m_lookat;
	XMVECTOR m_up;
};

