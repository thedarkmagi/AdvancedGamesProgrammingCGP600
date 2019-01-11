#include "ModelManager.h"
#include "objfilemodel.h"

ModelManager * ModelManager::instance = nullptr;

ModelManager::ModelManager()
{
}


ModelManager::~ModelManager()
{
}
ObjFileModel * ModelManager::getModel(char * input, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	if (modelMap.find(input) != modelMap.end())
	{
		return modelMap[input];
	}
	else
	{
		// create model - return it 
		// + add to list 
		ObjFileModel* temp = new ObjFileModel(input, device, deviceContext);
		map<char*,ObjFileModel*>::iterator it = modelMap.begin();
		modelMap.insert(it, pair<char*, ObjFileModel*>(input, temp));
		//modelMap.insert(input, temp);
		
		
		return temp;
	}


	return nullptr;
}


