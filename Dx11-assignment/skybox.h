#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>


#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>
#include"camera.h"
class skybox
{
public:
	skybox(ID3D11Device* device, ID3D11DeviceContext* deviceContext, camera* pCamera);
	~skybox();
	HRESULT InitialiseGraphics();
	void Draw(XMMATRIX* view, XMMATRIX* projection);
	HRESULT AddTexture(char* filename);
private:
	ID3D11Device*			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11VertexShader*	m_pVShader;
	ID3D11PixelShader*	m_pPShader;
	ID3D11InputLayout*	m_pInputLayout;
	ID3D11Buffer*		m_pConstantBuffer;

	ID3D11ShaderResourceView* m_pTexture0;
	ID3D11SamplerState* m_pSampler0;

	//code required to render the inside of the cube
	ID3D11RasterizerState*	m_pRasterSolid = 0;
	ID3D11RasterizerState*	m_pRasterSkyBox = 0;
	ID3D11DepthStencilState*	m_pDepthWriteSolid = 0;
	ID3D11DepthStencilState*	m_pDepthWriteSkybox = 0;

	camera* m_pCamera;
	int constantBufferByteWidth; // must be a multiple of 16
};

