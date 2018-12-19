#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>

#include "camera.h"
#include "text2D.h"
#include "objfilemodel.h"
#include "Model.h"
#include "input.h"
#include "skybox.h"
#include "GameObject.h"
class GameManager
{
public:
	GameManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext, IDXGISwapChain* SwapChain, HWND* hWind, HINSTANCE* hInst,
		ID3D11RenderTargetView* BackBufferRTView, ID3D11DepthStencilView* ZBuffer);
	~GameManager();
	void RenderFrame(void);
	HRESULT InitialiseGraphics(void);
	HRESULT SetupAlphaBlendStates();
private:
	ID3D11Device *			m_pD3DDevice;
	ID3D11DeviceContext*	m_pImmediateContext;
	IDXGISwapChain*         g_pSwapChain;
	HINSTANCE* g_hInst;
	HWND*		g_hWnd;


	ID3D11VertexShader*	g_pVertexShader;
	ID3D11PixelShader*	g_pPixelShader;
	ID3D11InputLayout*	g_pInputLayout;

	ID3D11DepthStencilView* g_pZBuffer;
	ID3D11RenderTargetView* g_pBackBufferRTView;
	//lighting
	XMVECTOR g_directional_light_shines_from;
	XMVECTOR g_directional_light_colour;
	XMVECTOR g_ambient_light_colour;

	// testing stuff 
	Model *g_pModel;
	Model *g_pModel2;
	Model *g_pModel3;
	input *g_pInput;
	skybox *g_pSkybox;
	GameObject *g_pGameObject;

	float g_lightX = 1.5f;
	float g_DirectionalColours;
	Text2D* g_2DText; 
	camera* pCamera; 

	// alpha blending variables
	ID3D11BlendState* m_pAlphaBlendEnable;
	ID3D11BlendState* m_pAlphaBlendDisable;
};

