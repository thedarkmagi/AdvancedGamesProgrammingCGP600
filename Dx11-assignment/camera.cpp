#include "camera.h"



camera::camera(float x, float y, float z, float cameraRotation)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_camera_rotation = cameraRotation;

	m_dx = sin(m_camera_rotation *(XM_PI / 180));
	m_dz = cos(m_camera_rotation * (XM_PI / 180));
	up();
}


camera::~camera()
{
}

void camera:: rotate(float nDegrees)
{
	m_camera_rotation += nDegrees;
	m_dx = sin(m_camera_rotation *(XM_PI / 180));
	m_dz = cos(m_camera_rotation * (XM_PI / 180));
}

void camera::rotatePitch(float nDegrees)
{
	m_camera_rotationX += nDegrees;

	m_dy = sin(m_camera_rotationX *(XM_PI / 180));
}


void camera::forward(float distance)
{
	m_x += distance * m_dx *GameTimer::getInstance()->DeltaTime();
	m_z += distance * m_dz*GameTimer::getInstance()->DeltaTime();
}

void camera::up()
{
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
}

void camera::strafe(float distance)
{
	XMVECTOR direction;
	direction = XMVector3Cross(m_up, m_lookat-m_position);
	//direction = XMVector3Cross(m_up,direction);
	m_x += direction.x * distance*GameTimer::getInstance()->DeltaTime();// *m_dx;
	m_z += direction.z * distance*GameTimer::getInstance()->DeltaTime();// *m_dz;
}

void camera::moveUp(float distance)
{
	m_y += distance;
}

void camera::rotateInX(float nDegrees)
{
	m_camera_rotationX += nDegrees;
	if (m_camera_rotationX > 90)
	{
		m_camera_rotationX = 89;
	}
	if (m_camera_rotationX < -90)
	{
		m_camera_rotationX = -89;
	}
	m_dy = sin(m_camera_rotationX *(XM_PI / 180));
}


XMMATRIX camera::GetViewMatix()
{
	
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	//m_position = XMVector3Normalize(m_position);
	m_lookat = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR view;
	//view = XMMatrixLookAtLH(m_position, m_lookat, m_up);
	return XMMatrixLookAtLH(m_position, m_lookat, m_up);
}

XMVECTOR camera::GetCameraPos()
{
	return m_position;
}

float camera::getX()
{
	return m_x;
}

float camera::getY()
{
	return m_y;
}

float camera::getZ()
{
	return m_z;
}

float camera::getDX()
{
	return m_dx;
}

float camera::getDY()
{
	return m_dy;
}

float camera::getDZ()
{
	return m_dz;
}
