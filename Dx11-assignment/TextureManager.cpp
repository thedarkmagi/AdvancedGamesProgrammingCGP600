#include "TextureManager.h"
#include <sstream>
TextureManager * TextureManager::instance = nullptr;

TextureManager:: TextureNSampler * TextureManager::getTexture(char * input, ID3D11Device * device, bool TwoTextures)
{
	if (TwoTextures)
	{
		string file1;
		string file2;
	}
	if (textureMap.find(input) != textureMap.end())
	{
		return textureMap[input];
	}
	else
	{
		TextureNSampler* temp = new TextureNSampler();

		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(sampler_desc));
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		device->CreateSamplerState(&sampler_desc, &temp->m_pSampler0);

		D3DX11CreateShaderResourceViewFromFile(device, input, NULL, NULL, &temp->m_pTexture0, NULL);


		map<char*, TextureNSampler*>::iterator it = textureMap.begin();
		textureMap.insert(it, pair<char*, TextureNSampler*>(input, temp));
		return temp;
	}

	return nullptr;
}
TextureManager::TextureNSampler * TextureManager::getTexture(char * input, char * input2, ID3D11Device * device, bool TwoTextures)
{
	std::stringstream ss;
	ss << input << input2;
	string keyInput = ss.str();
	if (textureMap.find((char*)keyInput.c_str()) != textureMap.end())
	{
		return textureMap[(char*)keyInput.c_str()];
	}
	else
	{
		TextureNSampler* temp = new TextureNSampler();

		D3D11_SAMPLER_DESC sampler_desc;
		ZeroMemory(&sampler_desc, sizeof(sampler_desc));
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		device->CreateSamplerState(&sampler_desc, &temp->m_pSampler0);

		D3DX11CreateShaderResourceViewFromFile(device, input, NULL, NULL, &temp->m_pTexture0, NULL);
		D3DX11CreateShaderResourceViewFromFile(device, input2, NULL, NULL, &temp->m_pTexture1, NULL);

		map<char*, TextureNSampler*>::iterator it = textureMap.begin();
		textureMap.insert(it, pair<char*, TextureNSampler*>((char*)keyInput.c_str(), temp));
		return temp;
	}

	return nullptr;
}

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}
