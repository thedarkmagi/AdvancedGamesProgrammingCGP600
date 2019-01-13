#include "GameManager.h"



GameManager::GameManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext, IDXGISwapChain* SwapChain, HWND* hWind, HINSTANCE* hInst,
	ID3D11RenderTargetView* BackBufferRTView, ID3D11DepthStencilView* ZBuffer)
{
	m_pD3DDevice = device;
	m_pImmediateContext = deviceContext;
	g_pSwapChain = SwapChain;
	g_hWnd = hWind;
	g_hInst = hInst;

	g_pBackBufferRTView = BackBufferRTView;
	g_pZBuffer = ZBuffer;
	g_DirectionalColours = 0.8f;
	g_pInput = new input(g_hInst, g_hWnd);
	g_pInput->initialise();
	SetupAlphaBlendStates();
}


GameManager::~GameManager()
{
	if (g_2DText) delete(g_2DText);

	if (pCamera) delete(pCamera);

	if (g_pInputLayout) g_pInputLayout->Release();
	if (g_pVertexShader) g_pVertexShader->Release();
	if (g_pPixelShader) g_pPixelShader->Release();
	if (g_pBackBufferRTView) g_pBackBufferRTView->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pD3DDevice) m_pD3DDevice->Release();
}

void GameManager::RenderFrame(void)
{
	g_pInput->ReadInputStates();
	XMMATRIX identity = XMMatrixIdentity();
	g_root_node->updateCollisionTree(&identity, 1.0);
	if (g_pInput->IsKeyPressed(DIK_ESCAPE))
	{
		DestroyWindow(*g_hWnd);
	}
	if (g_pInput->IsKeyPressed(DIK_W))
	{
		ObjFileModel::xyz oldPos;
		oldPos.x = pCamera->getX();
		oldPos.y = pCamera->getY();
		oldPos.z = pCamera->getZ();
		pCamera->forward(1.002f);
		// set camera node to the position of the camera
		g_cam_node->SetXPos(pCamera->getX());
		g_cam_node->SetYPos(pCamera->getY());
		g_cam_node->SetZPos(pCamera->getZ());

		ObjFileModel::xyz newPos;
		newPos.x = pCamera->getX();
		newPos.y = pCamera->getY();
		newPos.z = pCamera->getZ();

		XMMATRIX identity = XMMatrixIdentity();

		// update tree to reflect new camera position
		g_root_node->updateCollisionTree(&identity, 1.0);

		if (g_cam_node->checkCollision(g_root_node, true) == true)
		{
			if (g_root_node->checkCollisionRay(&oldPos, &newPos, g_cam_node, true) == true)
			{
				// if there is a collision, restore camera and camera node positions
				pCamera->forward(-1.002f);
				g_cam_node->SetXPos(pCamera->getX()); //15
				g_cam_node->SetYPos(pCamera->getY());//15
				g_cam_node->SetZPos(pCamera->getZ());//15

			}
		}

	}
	if (g_pInput->IsKeyPressed(DIK_S))
	{
		ObjFileModel::xyz oldPos;
		oldPos.x = pCamera->getX();
		oldPos.y = pCamera->getY();
		oldPos.z = pCamera->getZ();

		pCamera->forward(-1.002f);
		// set camera node to the position of the camera
		g_cam_node->SetXPos(pCamera->getX());
		g_cam_node->SetYPos(pCamera->getY());
		g_cam_node->SetZPos(pCamera->getZ());

		ObjFileModel::xyz newPos;
		newPos.x = pCamera->getX();
		newPos.y = pCamera->getY();
		newPos.z = pCamera->getZ();

		XMMATRIX identity = XMMatrixIdentity();

		// update tree to reflect new camera position
		g_root_node->updateCollisionTree(&identity, 1.0);

		if (g_cam_node->checkCollision(g_root_node,true) == true)
		{
			if (g_root_node->checkCollisionRay(&oldPos, &newPos, g_cam_node, true) == true)
			{
				// if there is a collision, restore camera and camera node positions
				pCamera->forward(1.002f);
				g_cam_node->SetXPos(pCamera->getX());//15
				g_cam_node->SetYPos(pCamera->getY());//15
				g_cam_node->SetZPos(pCamera->getZ());//15

			}
		}
	}
	if (g_pInput->IsKeyPressed(DIK_A))
	{
		pCamera->strafe(-1.002f);
		// set camera node to the position of the camera
		g_cam_node->SetXPos(pCamera->getX());
		g_cam_node->SetYPos(pCamera->getY());
		g_cam_node->SetZPos(pCamera->getZ());

		XMMATRIX identity = XMMatrixIdentity();

		// update tree to reflect new camera position
		g_root_node->updateCollisionTree(&identity, 1.0);

		if (g_cam_node->checkCollision(g_root_node, true) == true)
		{
			// if there is a collision, restore camera and camera node positions
			pCamera->strafe(1.002f);
			g_cam_node->SetXPos(pCamera->getX()); //15
			g_cam_node->SetYPos(pCamera->getY());//15
			g_cam_node->SetZPos(pCamera->getZ());//15

		}
	}
	if (g_pInput->IsKeyPressed(DIK_D))
	{
		pCamera->strafe(1.002f);
		// set camera node to the position of the camera
		g_cam_node->SetXPos(pCamera->getX());
		g_cam_node->SetYPos(pCamera->getY());
		g_cam_node->SetZPos(pCamera->getZ());

		XMMATRIX identity = XMMatrixIdentity();

		// update tree to reflect new camera position
		g_root_node->updateCollisionTree(&identity, 1.0);

		if (g_cam_node->checkCollision(g_root_node, true) == true)
		{
			// if there is a collision, restore camera and camera node positions
			pCamera->strafe(-0.002f);
			g_cam_node->SetXPos(pCamera->getX()); //15
			g_cam_node->SetYPos(pCamera->getY());//15
			g_cam_node->SetZPos(pCamera->getZ());//15

		}
	}
	if (g_pInput->IsKeyPressed(DIK_TAB))
	{
		if (!changeCamera)
		{
			changeCamera = true;
		}
		else
		{
			changeCamera = false;
		}
	}
	if (g_pInput->IsKeyPressed(DIK_SPACE))
	{
		if (pCamera->getY() < 1.75f)
		{
			pCamera->moveUp(5.002f);
			// set camera node to the position of the camera
			g_cam_node->SetXPos(pCamera->getX());
			g_cam_node->SetYPos(pCamera->getY());
			g_cam_node->SetZPos(pCamera->getZ());

			g_pParticleGenerator->SetXPos(pCamera->getX());// +(pCamera->getDX()) * 2);
			g_pParticleGenerator->SetYPos(pCamera->getY());// + pCamera->getDY() * 2);
			g_pParticleGenerator->SetZPos(pCamera->getZ());// +pCamera->getDX() * 2);
			g_pParticleGenerator->setIsActive(true);
			XMMATRIX identity = XMMatrixIdentity();

			// update tree to reflect new camera position
			g_root_node->updateCollisionTree(&identity, 1.0);
		}
	}
	else if (pCamera->getY() >0)
	{
		pCamera->moveUp(-2.5f);
		// set camera node to the position of the camera
		g_cam_node->SetXPos(pCamera->getX());
		g_cam_node->SetYPos(pCamera->getY());
		g_cam_node->SetZPos(pCamera->getZ());

		g_pParticleGenerator->setIsActive(false);
		// update tree to reflect new camera position
		g_root_node->updateCollisionTree(&identity, 1.0);
	}
	pCamera->rotate(g_pInput->GetMouseX());
	pCamera->rotateInX(g_pInput->GetMouseY());

	g_2DText->AddText("good luck FPS "+ std::to_string((int)(1.0f/GameTimer::getInstance()->DeltaTime())), -1.0, +1.0, 16 / 9 * 0.1);
	
	// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);

	m_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX world, projection, view;
	world = XMMatrixIdentity();
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 0.1, 1000);
	if (!changeCamera)
	{
		view = pCamera->GetViewMatix();
	}
	else
	{
		view = pCamera2->GetViewMatix();
	}
	g_pSkybox->Draw(&view, &projection);
	
	//lighting
	g_directional_light_shines_from = XMVectorSet(g_lightX, 0.0f, -1.0f, 0.0f);
	g_directional_light_colour = XMVectorSet(g_DirectionalColours, g_DirectionalColours, g_DirectionalColours, 1.0f); 
	g_ambient_light_colour = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);//dark grey - always use a small value for ambient lighting




	m_LevelManager->passCameraPos(pCamera->getX(), pCamera->getZ());
	m_LevelManager->update(&world, &view, &projection, &g_directional_light_colour, &g_directional_light_shines_from, &g_ambient_light_colour);
	//m_LevelManager->update(&world, &view, &projection);

	//Change blend type to enable alphablending
	m_pImmediateContext->OMSetBlendState(m_pAlphaBlendEnable, 0, 0xfffffff);
	g_pParticleGenerator->Draw(&view, &projection, &pCamera->GetCameraPos());
	g_2DText->RenderText();
	m_pImmediateContext->OMSetBlendState(m_pAlphaBlendDisable, 0, 0xfffffff);
	// Display what has just been rendered
	g_pSwapChain->Present(0, 0);
}

HRESULT GameManager::InitialiseGraphics(void)
{
	HRESULT hr = S_OK;


	pCamera = new camera(0.0, 0.0, -0.5, 0.0);
	pCamera2 = new camera(0, 10, 0, 0.0);
	
	g_pSkybox = new skybox(m_pD3DDevice, m_pImmediateContext, pCamera);
	hr = g_pSkybox->InitialiseGraphics();
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}

	hr = g_pSkybox->AddTexture((char*)"assets/DaylightSkybox.dds");
	if (FAILED(hr))
	{
		return hr;
	}
	g_pParticleGenerator = new ParticleGenerator(m_pD3DDevice, m_pImmediateContext);
	g_pParticleGenerator->ParticleFactory();
	g_pParticleGenerator->setIsActive(false);
	g_2DText = new Text2D("assets/font3.png", m_pD3DDevice, m_pImmediateContext);

	g_camObject = new GameObject(m_pD3DDevice, m_pImmediateContext);
	g_camObject->CreateModel((char*)"assets/Sphere.obj", (char*)"assets/texture.bmp");

	g_root_node = new SceneNode();
	g_cam_node = new SceneNode();
	g_cam_node->SetGameObject(g_camObject);
	g_root_node->addChildNode(g_cam_node);

	m_LevelManager = new LevelManager(m_pD3DDevice, m_pImmediateContext, g_root_node, g_cam_node);
	m_LevelManager->ReadFromFile("assets/level.txt");

	pCamera2->rotatePitch(-90.0f);
	g_cam_node->SetXPos(pCamera->getX()); //15
	g_cam_node->SetYPos(pCamera->getY());//15
	g_cam_node->SetZPos(pCamera->getZ());//15
	return hr;
}

HRESULT GameManager::SetupAlphaBlendStates()
{
	D3D11_BLEND_DESC blend;
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blend.IndependentBlendEnable = false;
	blend.AlphaToCoverageEnable = false;
	m_pD3DDevice->CreateBlendState(&blend, &m_pAlphaBlendEnable);
	return E_NOTIMPL;
}
