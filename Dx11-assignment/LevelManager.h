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
	LevelManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext, SceneNode* rootNode);
	~LevelManager();

	void ReadFromFile(string input);
	void update(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection, XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour);
	void update(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection);

	void passCameraPos(float x, float z);
private:
	vector<string> list;
	vector<SceneNode*> nodeHierarchy;
	vector<GameObject*> gameObjectList;
	SceneNode* m_rootNode;

	ID3D11Device *			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	//temp VALUES PLEASE REPLACE LATER
	float cameraX, cameraZ;
};

