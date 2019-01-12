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

		//if (g_cam_node->checkCollision(g_root_node) == true)
		//{
		//	// if there is a collision, restore camera and camera node positions
		//	pCamera->forward(-0.002f);
		//	g_cam_node->SetXPos(pCamera->getX()); //15
		//	g_cam_node->SetYPos(pCamera->getY());//15
		//	g_cam_node->SetZPos(pCamera->getZ());//15

		//}
		if (g_cam_node->checkCollision(g_root_node) == true)
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

		//if (g_cam_node->checkCollision(g_root_node) == true)
		//{
		//	// if there is a collision, restore camera and camera node positions
		//	pCamera->forward(0.002f);
		//	g_cam_node->SetXPos(pCamera->getX()); //15
		//	g_cam_node->SetYPos(pCamera->getY());//15
		//	g_cam_node->SetZPos(pCamera->getZ());//15

		//}
		if (g_cam_node->checkCollision(g_root_node) == true)
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

		if (g_cam_node->checkCollision(g_root_node) == true)
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

		if (g_cam_node->checkCollision(g_root_node) == true)
		{
			// if there is a collision, restore camera and camera node positions
			pCamera->strafe(-0.002f);
			g_cam_node->SetXPos(pCamera->getX()); //15
			g_cam_node->SetYPos(pCamera->getY());//15
			g_cam_node->SetZPos(pCamera->getZ());//15

		}
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


	//g_2DText->AddText(to_string( GameTimer::getInstance()->GameTime()), -1.0, +1.0,  16/9 *0.2);
	g_2DText->AddText("good luck"+ std::to_string((int)(1.0f/GameTimer::getInstance()->DeltaTime())), -1.0, +1.0, 16 / 9 * 0.1);
	// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.1f, 0.2f, 0.6f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);

	m_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX world, projection2, view2;
	world = XMMatrixIdentity();
	projection2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 1.0, 250.0);
	view2 = pCamera->GetViewMatix();
	g_pSkybox->Draw(&view2, &projection2);
	
	//Some things I need to work out
	//m_pImmediateContext->VSSetShader(g_pVertexShader, 0, 0);
	//m_pImmediateContext->PSSetShader(g_pPixelShader, 0, 0);
	//m_pImmediateContext->IASetInputLayout(g_pInputLayout);

	//lighting
	g_directional_light_shines_from = XMVectorSet(g_lightX, 0.0f, -1.0f, 0.0f);
	g_directional_light_colour = XMVectorSet(g_DirectionalColours, g_DirectionalColours, g_DirectionalColours, 1.0f);//WHITE??? 
	g_ambient_light_colour = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);//dark grey - always use a small value for ambient lighting


	g_pModel->setdirectionalLightColour(&g_directional_light_colour);
	g_pModel->setDirectionLightVector(&g_directional_light_shines_from);
	g_pModel->setAmbientLightColour(&g_ambient_light_colour);

	//g_pModel->lookAt_XZ(pCamera->getX(), pCamera->getZ());
	//g_pModel->lookAt_XZ(g_pModel3->GetXPos(), g_pModel3->GetZPos());
	//g_pModel->moveForward(0.001f);

	if (g_pModel->CheckCollision(g_pModel3))
	{
		DebugBreak;
		//g_pModel->moveForward(-0.001f);
	}
	//g_pModel->Draw(&view2, &projection2);
	projection2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 1.0, 250.0);
	view2 = pCamera->GetViewMatix();
	/*g_pModel2->setdirectionalLightColour(&g_directional_light_colour);
	g_pModel2->setDirectionLightVector(&g_directional_light_shines_from);
	g_pModel2->setAmbientLightColour(&g_ambient_light_colour);
*/
	//g_pModel2->lookAt_XZ(pCamera->getX(), pCamera->getZ());
	//g_pModel->moveForward(0.001f);
	//g_pModel2->Draw(&view2, &projection2);

	projection2 = XMMatrixPerspectiveFovLH(XMConvertToRadians(30.0), 1920.0 / 1080.0, 1.0, 250.0);
	view2 = pCamera->GetViewMatix();
	g_pModel3->setdirectionalLightColour(&g_directional_light_colour);
	g_pModel3->setDirectionLightVector(&g_directional_light_shines_from);
	g_pModel3->setAmbientLightColour(&g_ambient_light_colour);

	/*g_node1->SetXPos(10.0f);
	g_node2->SetYPos(10.0f);*/
	g_root_node->execute(&world, &view2, &projection2, &g_directional_light_colour, &g_directional_light_shines_from, &g_ambient_light_colour);
	//g_pModel3->lookAt_XZ(g_pModel->GetXPos(), g_pModel->GetZPos());
	//g_pModel3->moveForward(0.001f);

	if (g_pModel3->CheckCollision(g_pModel))
	{
		DebugBreak;
		//g_pModel3->moveForward(-0.001f);

	}
	//g_pModel3->Draw(&view2, &projection2);
	//g_pGameObject->setLightingValues(&g_directional_light_shines_from, &g_directional_light_colour, &g_ambient_light_colour);
	//g_pGameObject->update(&view2, &projection2);
	//g_pParticleGenerator->lookAt_XZ(pCamera->getX(), pCamera->getZ());

	m_LevelManager->passCameraPos(pCamera->getX(), pCamera->getZ());
	m_LevelManager->update(&world, &view2, &projection2, &g_directional_light_colour, &g_directional_light_shines_from, &g_ambient_light_colour);
	//m_LevelManager->update(&world, &view2, &projection2);
	//render text
	m_pImmediateContext->OMSetBlendState(m_pAlphaBlendEnable, 0, 0xfffffff);
	g_pParticleGenerator->Draw(&view2, &projection2, &pCamera->GetCameraPos());
	g_2DText->RenderText();
	m_pImmediateContext->OMSetBlendState(m_pAlphaBlendDisable, 0, 0xfffffff);
	// Display what has just been rendered
	g_pSwapChain->Present(0, 0);
}

HRESULT GameManager::InitialiseGraphics(void)
{
	HRESULT hr = S_OK;

	

	g_pModel = new Model(m_pD3DDevice, m_pImmediateContext);
	hr = g_pModel->LoadObjModel((char*)"assets/Sphere.obj",0);
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}
	//g_pModel2 = new Model(m_pD3DDevice, m_pImmediateContext);
	//hr = g_pModel2->LoadObjModel((char*)"assets/plane.obj", 0);
	//if (FAILED(hr))//Return an error code if failed
	//{
	//	return hr;
	//}
	g_pModel3 = new Model(m_pD3DDevice, m_pImmediateContext);
	hr = g_pModel3->LoadObjModel((char*)"assets/PointySphere.obj", 0);
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}

	pCamera = new camera(0.0, 0.0, -0.5, 0.0);
	g_pSkybox = new skybox(m_pD3DDevice, m_pImmediateContext, pCamera);
	hr = g_pSkybox->InitialiseGraphics();
	if (FAILED(hr))//Return an error code if failed
	{
		return hr;
	}

	g_pGameObject = new GameObject(m_pD3DDevice, m_pImmediateContext);
	hr = g_pGameObject->CreateModel((char*)"assets/teapot.obj", (char*)"assets/texture.bmp", (char*)"assets/texture3.png");
	if (FAILED(hr))
	{
		return hr;
	}

	hr = g_pModel->AddTexture((char*)"assets/texture.bmp");
	if (FAILED(hr))
	{
		return hr;
	}
	//hr = g_pModel2->AddTexture((char*)"assets/texture.bmp");
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
	g_pParticleGenerator = new ParticleGenerator(m_pD3DDevice, m_pImmediateContext);
	g_pParticleGenerator->ParticleFactory();
	g_pParticleGenerator->setIsActive(true);
	g_2DText = new Text2D("assets/font3.png", m_pD3DDevice, m_pImmediateContext);

	g_camObject = new GameObject(m_pD3DDevice, m_pImmediateContext);
	g_camObject->CreateModel((char*)"assets/Sphere.obj", (char*)"assets/texture.bmp");

	g_root_node = new SceneNode();
	//g_node1 = new SceneNode();
	//g_node2 = new SceneNode();
	g_cam_node = new SceneNode();
	//g_node1->SetModel(g_pModel);
	//g_node2->SetModel(g_pModel3);
	g_cam_node->SetGameObject(g_camObject);

	//g_root_node->addChildNode(g_node1);
	//g_node1->addChildNode(g_node2);
	g_root_node->addChildNode(g_cam_node);

	m_LevelManager = new LevelManager(m_pD3DDevice, m_pImmediateContext, g_root_node, g_cam_node);
	m_LevelManager->ReadFromFile("assets/level.txt");
	//g_pModel->SetZPos(10.0f);
	//g_pModel->SetXPos(10.0f);

	//g_pModel2->SetXPos(0.0f);
	//g_pModel2->SetYPos(-10.0f);
	//g_pModel2->SetZPos(0.0f);
	//g_pModel2->SetScale(100.0f);

	//g_pModel3->SetXPos(-10.0f);
	////g_pModel3->SetYPos(4.0f);
	//g_pModel2->SetZPos(10.0f);
	//g_pModel3->SetZPos(100.0f);
	//g_pModel3->SetScale(1.0f);
	pCamera->strafe(-1);
	g_cam_node->SetXPos(pCamera->getX()); //15
	g_cam_node->SetYPos(pCamera->getY());//15
	g_cam_node->SetZPos(pCamera->getZ());//15

	//pCamera->rotateInX(3);
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
