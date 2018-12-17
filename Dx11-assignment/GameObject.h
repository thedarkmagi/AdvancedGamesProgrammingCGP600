#pragma once
#include"Model.h"
class GameObject
{
public:
	GameObject();
	GameObject(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~GameObject();

	HRESULT CreateModel(char * ModelFilename, char * TextureFilename);
	HRESULT CreateModel(char * ModelFilename, char * TextureFilename, char * TextureFilename2);
	void update(XMMATRIX* view, XMMATRIX* projection);
	void setLightingValues(XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour);
private:
	ID3D11Device *			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	Model* m_pModel;

};

