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

class TextureManager
{
public:
	struct TextureNSampler
	{
		ID3D11ShaderResourceView* m_pTexture0;
		ID3D11ShaderResourceView* m_pTexture1;
		ID3D11SamplerState* m_pSampler0;
	};

	static TextureManager* instance;
	static TextureManager* getInstance()
	{
		if (!instance)
		{
			instance = new TextureManager();
		}
		return instance;
	}
	TextureManager(TextureManager const&) = delete;
	void operator=(TextureManager const&) = delete;

	TextureNSampler* getTexture(char* input, ID3D11Device * device, bool TwoTextures);
	TextureNSampler* getTexture(char* input, char* input2, ID3D11Device * device, bool TwoTextures);

private:
	TextureManager();
	~TextureManager();

	map<char*, TextureNSampler*> textureMap;
};

