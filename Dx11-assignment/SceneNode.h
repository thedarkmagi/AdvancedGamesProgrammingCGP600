#pragma once
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "Model.h"
#include "GameObject.h"
class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	void addChildNode(SceneNode *n);
	bool detatchNode(SceneNode *n);
	void execute(XMMATRIX *world, XMMATRIX* view, XMMATRIX* projection, XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour);

#pragma region Gets & Sets
	void SetXPos(float num);
	void SetYPos(float num);
	void SetZPos(float num);

	float GetXPos();
	float GetYPos();
	float GetZPos();

	void SetXRot(float num);
	void SetYRot(float num);
	void SetZRot(float num);

	float GetXRot();
	float GetYRot();
	float GetZRot();

	void SetScale(float num);
	float GetScale();
	void SetModel(Model* m);
	void SetGameObject(GameObject* m);
#pragma endregion
private:
	Model * m_pModel;
	GameObject * m_pGameObject;
	vector<SceneNode*> m_children;
	float			m_x, m_y, m_z;
	float			m_xAngle, m_yAngle, m_zAngle;
	float			m_scale;
};

