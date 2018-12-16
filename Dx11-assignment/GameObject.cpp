#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::GameObject(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_pD3DDevice = device;
	m_pImmediateContext = deviceContext;
	
}


GameObject::~GameObject()
{
}

HRESULT GameObject::CreateModel(char * ModelFilename, char * TextureFilename)
{
	HRESULT hr = S_OK;
	m_pModel = new Model(m_pD3DDevice, m_pImmediateContext);
	hr = m_pModel->LoadObjModel(ModelFilename);
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}
	hr = m_pModel->AddTexture(TextureFilename);
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}
	return hr;
}

void GameObject::update(XMMATRIX* view, XMMATRIX* projection)
{
	m_pModel->Draw(view, projection);
}

void GameObject::setLightingValues(XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour)
{
	m_pModel->setDirectionLightVector(directionLightVector);
	m_pModel->setdirectionalLightColour(lightColour);
	m_pModel->setAmbientLightColour(ambientLightColour);
}
