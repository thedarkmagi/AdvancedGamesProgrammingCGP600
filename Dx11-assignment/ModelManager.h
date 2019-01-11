#pragma once
#include <map>

#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>

using namespace std;
class ObjFileModel;
class ModelManager
{
public:


	static ModelManager* instance;
	static ModelManager* getInstance()
	{
		if (!instance)
		{
			instance = new ModelManager();
		}
		return instance;
	}
	ModelManager(ModelManager const&) = delete;
	void operator=(ModelManager const&) = delete;

	ObjFileModel* getModel(char* input, ID3D11Device * device, ID3D11DeviceContext * deviceContext);

private:
	ModelManager();
	~ModelManager();

	map<char*, ObjFileModel*> modelMap;
};

