#include "LevelManager.h"



LevelManager::LevelManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext, SceneNode* rootNode, SceneNode* camNode)
{
	m_pD3DDevice = device;
	m_pImmediateContext = deviceContext;
	m_rootNode = rootNode;
	m_camNode = camNode;
	//m_rootNode->addChildNode(m_camNode);
}


LevelManager::~LevelManager()
{
}

void LevelManager::ReadFromFile(string input)
{
	string line; 
	ifstream levelFile(input);
	if (levelFile.is_open())
	{
		//list->size = levelFile.ios::end;
		//list
		int i = 0;
		while (getline(levelFile, line))
		{
			
			list.push_back(line);
			i++;
		}
		levelFile.close();

		//m_rootNode = new SceneNode();

		for (int i = 0; i < list.size(); i++)
		{
			for (int j = 0; j < list[i].size(); j++)
			{
				char a= list[i][j];
				GameObject * temp;
				SceneNode * tempNode = new SceneNode();
				switch (list[i][j])
				{
					case 'w':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/cube.obj", (char*)"assets/texture.bmp");
						//questionable if needed
						//gameObjectList.push_back(temp);
						//tempNode->SetModel(temp->getModel());#
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i*2);
						tempNode->SetZPos(j*2);
						break;
					case 'c':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/cube.obj", (char*)"assets/texture3.bmp", (char*)"assets/texture3.png");
						//questionable if needed
						//gameObjectList.push_back(temp);
						//tempNode->SetModel(temp->getModel());#
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2);
						tempNode->SetZPos(j * 2);

						//tempNode->SetScale(2.0f);
						nodeHierarchy.push_back(tempNode);
						break;
					case 'e':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/Sphere.obj", (char*)"assets/texture.bmp");
						gameObjectList.push_back(temp);
						
						//tempNode->SetModel(temp->getModel());#
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2);
						tempNode->SetZPos(j * 2);
						tempNode->SetScale(0.2f);
						//nodeHierarchy.push_back(tempNode);
						break;
					case 'p':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/giantRobot.obj", (char*)"assets/1pPink.bmp");
						gameObjectList.push_back(temp);

						//tempNode->SetModel(temp->getModel());#
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2);
						tempNode->SetZPos(j * 2);
						tempNode->SetScale(0.5f);
						nodeHierarchy.push_back(tempNode);
						break;
					case 's':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/PointySphere.obj", (char*)"assets/texture.bmp");
						gameObjectList.push_back(temp);

						//tempNode->SetModel(temp->getModel());#
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2);
						tempNode->SetZPos(j * 2);
						tempNode->SetScale(0.2f);
						nodeHierarchy.push_back(tempNode);
						break;
					default:
						break;
				}
			}
		}
	}

}

void LevelManager::update(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection, XMVECTOR * directionLightVector, XMVECTOR * lightColour, XMVECTOR * ambientLightColour)
{
	//for (int i = 0; i < gameObjectList.size(); i++)
	//{
	//	gameObjectList[i]->setLightingValues(directionLightVector, lightColour, ambientLightColour);
	//	gameObjectList[i]->update(world, view, projection);
	//}
	for (int i = 0; i < nodeHierarchy.size(); i++)
	{
		//nodeHierarchy[i]->incX(1*GameTimer::getInstance()->DeltaTime(), m_rootNode);
		//nodeHierarchy[i]->lookAt_XZ(cameraX, cameraZ);
		nodeHierarchy[i]->moveForward(1, m_rootNode);
	}
	m_rootNode->execute(world, view, projection, directionLightVector, lightColour, ambientLightColour);
}

void LevelManager::update(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection)
{
	
}

void LevelManager::passCameraPos(float x, float z)
{
	cameraX = x;
	cameraZ = z;
}
