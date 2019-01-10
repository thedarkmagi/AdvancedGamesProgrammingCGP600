#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>

#include <stdio.h>
#include <string>
#include <vector>

#include "Model.h"
#include "GameObject.h"
#include "maths.h"
class SceneNode
{
public:
	SceneNode();
	~SceneNode();

	void addChildNode(SceneNode *n);
	bool detatchNode(SceneNode *n);
	void execute(XMMATRIX *world, XMMATRIX* view, XMMATRIX* projection, XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour);
	void updateCollisionTree(XMMATRIX* world, float scale);
	bool checkCollision(SceneNode* compare_tree);
	bool checkCollision(SceneNode* compare_tree, SceneNode* object_tree_root);
	bool checkCollisionRay(ObjFileModel::xyz* ray, ObjFileModel::xyz* rayDirection, SceneNode* compare_tree);
	bool checkCollisionRay(ObjFileModel::xyz* ray, ObjFileModel::xyz* rayDirection, SceneNode* compare_tree, SceneNode* object_tree_root);
	bool lookAt_XZ(float x, float z);
	bool lookAt_XZ(float x, float z, SceneNode* root_node);
	bool moveForward(float amount);
	bool moveForward(float amount, SceneNode* root_node);
#pragma region Gets & Sets
	void SetXPos(float num);
	void SetYPos(float num);
	void SetZPos(float num);

	float GetXPos();
	float GetYPos();
	float GetZPos();

	bool incX(float in, SceneNode* root_node);
	bool incY(float in, SceneNode* root_node);
	bool incZ(float in, SceneNode* root_node);

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
	XMVECTOR getWorldCentrePosition();

	ObjFileModel::xyz XMVecToXYZ(XMVECTOR input);
	XMVECTOR XYZToXMVec(ObjFileModel::xyz* input);
	ObjFileModel::xyz addTogether(ObjFileModel::xyz* one, ObjFileModel::xyz* two);
#pragma endregion
private:
	Model * m_pModel;
	GameObject * m_pGameObject;
	vector<SceneNode*> m_children;
	float			m_x, m_y, m_z;
	float			m_xAngle, m_yAngle, m_zAngle;
	float			m_scale;
	float			m_world_centre_x, m_world_centre_y, m_world_centre_z, m_world_scale;
	XMMATRIX m_local_world_matrix;
	maths*	localMath;
};

