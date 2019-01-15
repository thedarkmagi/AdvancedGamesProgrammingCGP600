#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "GameTimer.h"
#include "camera.h"
#include "text2D.h"
#include "objfilemodel.h"
#include "Model.h"
#include "input.h"
#include "skybox.h"
#include "GameObject.h"
#include "ParticleGenerator.h"
#include "SceneNode.h"
#include "LevelManager.h"

using namespace std;
class LevelManager
{
public:
	LevelManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext, SceneNode* rootNode, SceneNode* camNode);
	~LevelManager();

	void ReadFromFile(string input);
	void update(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection, XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour);
	void passCameraPos(float x, float z);
	void setCameraPointer(camera* cam);
private:
	vector<string> list;
	vector<SceneNode*> nodeHierarchy;
	vector<SceneNode*> enemyHierarchy;
	vector<GameObject*> gameObjectList;
	SceneNode* m_rootNode;
	SceneNode* m_camNode;
	ID3D11Device *			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	camera* m_pCamera;
	
	float cameraX, cameraZ;
};

