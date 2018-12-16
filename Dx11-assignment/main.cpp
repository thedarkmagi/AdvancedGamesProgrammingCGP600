//The #include order is important
#include <d3d11.h>
#include <dxgi.h>
#include <d3dx11.h>
#include <windows.h>
#include <dxerr.h>


#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT
#include <xnamath.h>
#include "GameManager.h"

#include "camera.h"
#include "text2D.h"
#include "objfilemodel.h"
#include "Model.h"
#include "input.h"
#include "skybox.h"
#include "GameObject.h"
using namespace std;
#pragma region Globals
	// globals //
	HINSTANCE g_hInst = NULL;
	HWND		g_hWnd = NULL;
	
	//will appear in the title bar of window
	char g_TutorialName[100] = "Assignment Boi's\0";
	D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           g_pD3DDevice = NULL;
	ID3D11DeviceContext*    g_pImmediateContext = NULL;
	IDXGISwapChain*         g_pSwapChain = NULL;

	//tutorial 2 ex 1 
	ID3D11RenderTargetView* g_pBackBufferRTView = NULL;
	//tutorial 3 ex 1
	ID3D11Buffer*		g_pVertexBuffer;
	ID3D11VertexShader*	g_pVertexShader;
	ID3D11PixelShader*	g_pPixelShader;
	ID3D11InputLayout*	g_pInputLayout;
	
	//tutorial 8 
	ID3D11ShaderResourceView* g_pTexture0;
	ID3D11SamplerState* g_pSampler0;
	//8 ex2
	Text2D* g_2DText;

	//9 ex1 
	XMVECTOR g_directional_light_shines_from;
	XMVECTOR g_directional_light_colour;
	XMVECTOR g_ambient_light_colour;

	//10 
	ObjFileModel *pObject;

	Model *g_pModel;
	Model *g_pModel2;
	Model *g_pModel3;
	input *g_pInput;
	skybox *g_pSkybox;
	GameObject *g_pGameObject;
	//XMVECTOR g_
	float g_lightX =1.5f;
	float g_DirectionalColours;
	//Define vertex structure




	//const int constBufferByteWidth = 128; // must be a multiple of 16
	//float redAmount; // for changing red input
	//float scaleAmout;
	//float degrees =15.0;
	
	//zBuffer
	ID3D11DepthStencilView* g_pZBuffer;

	camera* pCamera;
	GameManager* g_pGameManger;
#pragma endregion

#pragma region Forward Declarations
	//Forward declarations
	HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	HRESULT InitialiseD3D();
	void ShutdownD3D();
	void RenderFrame(void);
	HRESULT InitialiseGraphics(void);
#pragma endregion




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	if (FAILED(InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create window");
		return 0;
	}
	if (FAILED(InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}
	g_pGameManger = new GameManager(g_pD3DDevice, g_pImmediateContext, g_pSwapChain, &g_hWnd, &g_hInst, g_pBackBufferRTView, g_pZBuffer);
	if (FAILED(g_pGameManger->InitialiseGraphics()))
	{
		DXTRACE_MSG("Failed to Initialise Graphics");
		return 0;
	}
	//if (FAILED(InitialiseGraphics()))
	//{
	//	DXTRACE_MSG("Failed to Initialise Graphics");
	//	return 0;
	//}
	g_pInput = new input(&g_hInst, &g_hWnd);
	g_pInput->initialise();

#pragma region main message loop

	// main message loop
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_pGameManger->RenderFrame();
			//RenderFrame();
		}
	}


#pragma endregion
	ShutdownD3D();

	return (int) msg.wParam;
}

// register class and create window 
HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow)
{
	//APP name
	char Name[100] = "Hello My Debt Pit\0";

	//resgister classs
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	//   wcex.hbrBackground = (HBRUSH )( COLOR_WINDOW + 1); // Needed for non-D3D apps
	wcex.lpszClassName = Name;

	if (!RegisterClassEx(&wcex)) return E_FAIL;

	// Create window
	g_hInst = hInstance;
	RECT rc = { 0, 0, 1920, 1080 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(Name, g_TutorialName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left,
		rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);

	return S_OK;

}

//called everytime the application recieves a message 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
// Create D3D device and swap chain
//////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseD3D()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

//#ifdef _DEBUG
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; //this breaks the project if we in debug at UNI
//#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, // comment out this line if you need to test D3D 11.0 functionality on hardware that doesn't support it
		D3D_DRIVER_TYPE_WARP, // comment this out also to use reference device
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL,
			createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &g_pSwapChain,
			&g_pD3DDevice, &g_featureLevel, &g_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;
	// Get pointer to back buffer texture
	ID3D11Texture2D *pBackBufferTexture;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBufferTexture);

	if (FAILED(hr)) return hr;

	// Use the back buffer texture pointer to create the render target view
	hr = g_pD3DDevice->CreateRenderTargetView(pBackBufferTexture, NULL,
		&g_pBackBufferRTView);
	pBackBufferTexture->Release();

	if (FAILED(hr)) return hr;

	// Create a Z buffer texture
	D3D11_TEXTURE2D_DESC tex2dDesc;
	ZeroMemory(&tex2dDesc, sizeof(tex2dDesc));

	tex2dDesc.Width = width;
	tex2dDesc.Height = height;
	tex2dDesc.ArraySize = 1;
	tex2dDesc.MipLevels = 1;
	tex2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex2dDesc.SampleDesc.Count = sd.SampleDesc.Count;
	tex2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex2dDesc.Usage = D3D11_USAGE_DEFAULT;

	ID3D11Texture2D *pZBufferTexture;
	hr = g_pD3DDevice->CreateTexture2D(&tex2dDesc, NULL, &pZBufferTexture);

	if (FAILED(hr)) return hr;

	// create the z buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));

	dsvDesc.Format = tex2dDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	g_pD3DDevice->CreateDepthStencilView(pZBufferTexture, &dsvDesc, &g_pZBuffer);
	pZBufferTexture->Release();

	// Set the render target view
	g_pImmediateContext->OMSetRenderTargets(1, &g_pBackBufferRTView, g_pZBuffer);

	// Set the viewport
	D3D11_VIEWPORT viewport;

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	g_pImmediateContext->RSSetViewports(1, &viewport);

	g_2DText = new Text2D("assets/font1.bmp", g_pD3DDevice, g_pImmediateContext);

	
	/*pObject = new ObjFileModel((char*)"assets/Sphere.obj", g_pD3DDevice, g_pImmediateContext);
	if (pObject->filename == "FILE NOT LOADED")return S_FALSE;*/
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
// Clean up D3D objects
//////////////////////////////////////////////////////////////////////////////////////
void ShutdownD3D()
{
	if (pObject) delete pObject;
	if (g_2DText) delete(g_2DText);
	if (g_pTexture0) g_pTexture0->Release();
	if (g_pSampler0) g_pSampler0->Release();
	//tutorial 4 
	if (pCamera) delete(pCamera);


	if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pInputLayout) g_pInputLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pBackBufferRTView) g_pBackBufferRTView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pD3DDevice) g_pD3DDevice->Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////
//Init graphics - Tutorial 03
/////////////////////////////////////////////////////////////////////////////////////////////
HRESULT InitialiseGraphics()
{
	HRESULT hr = S_OK;

	g_pModel = new Model(g_pD3DDevice, g_pImmediateContext);
	hr =  g_pModel->LoadObjModel((char*)"assets/Sphere.obj");
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}
	g_pModel2 = new Model(g_pD3DDevice, g_pImmediateContext);
	hr = g_pModel2->LoadObjModel((char*)"assets/plane.obj");
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}
	g_pModel3 = new Model(g_pD3DDevice, g_pImmediateContext);
	hr = g_pModel3->LoadObjModel((char*)"assets/PointySphere.obj");
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}

	pCamera = new camera(0.0, 0.0, -0.5, 0.0);
	g_pSkybox = new skybox(g_pD3DDevice, g_pImmediateContext, pCamera);
	hr = g_pSkybox->InitialiseGraphics();
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}

	g_pGameObject = new GameObject(g_pD3DDevice, g_pImmediateContext);
	hr = g_pGameObject->CreateModel((char*)"assets/teapot.obj", (char*)"assets/texture.bmp");
	if (FAILED(hr))
	{
		return hr;
	}

	hr = g_pModel->AddTexture((char*)"assets/texture.bmp");
	if (FAILED(hr))
	{
		return hr;
	}
	hr = g_pModel2->AddTexture((char*)"assets/texture.bmp");
	if (FAILED(hr))
	{
		return hr;
	}
	hr = g_pModel3->AddTexture((char*)"assets/texture2.bmp");
	if (FAILED(hr))
	{
		return hr;
	}
	hr = g_pSkybox->AddTexture((char*)"assets/DaylightSkybox.dds");
	if (FAILED(hr))
	{
		return hr;
	}
	g_pModel->SetZPos(10.0f);
	g_pModel->SetXPos(10.0f);

	g_pModel2->SetXPos(0.0f);
	g_pModel2->SetYPos(-10.0f);
	g_pModel2->SetZPos(0.0f);
	g_pModel2->SetScale(100.0f);

	g_pModel3->SetXPos(-10.0f);
	//g_pModel3->SetYPos(4.0f);
	g_pModel2->SetZPos(10.0f);
	g_pModel3->SetZPos(100.0f);
	g_pModel3->SetScale(1.0f);

	pCamera->rotateInX(3);
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////////////////////


// Render frame
void RenderFrame(void)
{
	g_pInput->ReadInputStates();

	if (g_pInput->IsKeyPressed(DIK_ESCAPE))
	{
		DestroyWindow(g_hWnd);
	}
	if (g_pInput->IsKeyPressed(DIK_W))
	{
		pCamera->forward(0.002f);
	}
	if (g_pInput->IsKeyPressed(DIK_S))
	{
		pCamera->forward(-0.002f);
	}
	if (g_pInput->IsKeyPressed(DIK_A))
	{
		pCamera->strafe(-0.002f);
	}
	if (g_pInput->IsKeyPressed(DIK_D))
	{
		pCamera->strafe(0.002f);
	}
	pCamera->rotate(g_pInput->GetMouseX());
	pCamera->rotateInX(g_pInput->GetMouseY());
	
	/*if (g_pInput->IsMouseButtonPressed(0))
	{
		pCamera->moveUp(10.0f);
	}
	if (g_pInput->IsMouseButtonPressed(1))
	{
		pCamera->moveUp(-10.0f);
	}*/
	

	

	g_2DText->AddText("some Text", -1.0, +1.0, 0.2);
	// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);

	g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX projection2, view2;
	projection2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 1.0, 250.0);
	view2 = pCamera->GetViewMatix();
	g_pSkybox->Draw(&view2, &projection2);
	//Some things I need to work out
	g_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
	g_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);
	g_pImmediateContext->IASetInputLayout(g_pInputLayout);

	//lighting
	g_directional_light_shines_from = XMVectorSet(g_lightX, 0.0f, -1.0f, 0.0f);
	g_directional_light_colour = XMVectorSet(g_DirectionalColours, g_DirectionalColours, g_DirectionalColours, 1.0f);//WHITE??? 
	g_ambient_light_colour = XMVectorSet(1.1f, 1.1f, 1.1f, 1.0f);//dark grey - always use a small value for ambient lighting
	
	
	g_pModel->setdirectionalLightColour(&g_directional_light_colour);
	g_pModel->setDirectionLightVector(&g_directional_light_shines_from);
	g_pModel->setAmbientLightColour(&g_ambient_light_colour);

	//g_pModel->lookAt_XZ(pCamera->getX(), pCamera->getZ());
	g_pModel->lookAt_XZ(g_pModel3->GetXPos(), g_pModel3->GetZPos());
	//g_pModel->moveForward(0.001f);
	
	if (g_pModel->CheckCollision(g_pModel3))
	{
		DebugBreak;
		g_pModel->moveForward(-0.001f);
	}
	g_pModel->Draw(&view2,&projection2);
	projection2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 1.0, 250.0);
	view2 = pCamera->GetViewMatix();
	g_pModel2->setdirectionalLightColour(&g_directional_light_colour);
	g_pModel2->setDirectionLightVector(&g_directional_light_shines_from);
	g_pModel2->setAmbientLightColour(&g_ambient_light_colour);

	g_pModel2->lookAt_XZ(pCamera->getX(), pCamera->getZ());
	g_pModel->moveForward(0.001f);
	g_pModel2->Draw(&view2, &projection2);
	
	projection2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 1.0, 250.0);
	view2 = pCamera->GetViewMatix();
	g_pModel3->setdirectionalLightColour(&g_directional_light_colour);
	g_pModel3->setDirectionLightVector(&g_directional_light_shines_from);
	g_pModel3->setAmbientLightColour(&g_ambient_light_colour);

	g_pModel3->lookAt_XZ(g_pModel->GetXPos(), g_pModel->GetZPos());
	g_pModel3->moveForward(0.001f);

	if (g_pModel3->CheckCollision(g_pModel))
	{
		DebugBreak;
		g_pModel3->moveForward(-0.001f);

	}
	g_pModel3->Draw(&view2, &projection2);
	g_pGameObject->setLightingValues(&g_directional_light_shines_from, &g_directional_light_colour, &g_ambient_light_colour);
	g_pGameObject->update(&view2, &projection2);
	//render text
	g_2DText->RenderText();	
	// Display what has just been rendered
	g_pSwapChain->Present(0, 0);
}

