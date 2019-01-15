#include "LevelManager.h"



LevelManager::LevelManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext, SceneNode* rootNode, SceneNode* camNode)
{
	m_pD3DDevice = device;
	m_pImmediateContext = deviceContext;
	m_rootNode = rootNode;
	m_camNode = camNode;
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
		// reading file into vector 
		while (getline(levelFile, line))
		{
			
			list.push_back(line);
		}
		levelFile.close();

		// looping through the vector char by char 
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
						temp->getModel()->setCameraPointer(m_pCamera);

						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i*2-5);
						tempNode->SetZPos(j*2 - 5);
						break;
					case 'c':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/cube.obj", (char*)"assets/texture3.bmp", (char*)"assets/texture3.png");
						temp->getModel()->setCameraPointer(m_pCamera);

						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2 - 5);
						tempNode->SetZPos(j * 2 - 5);
						break;
					case 'e':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/Sphere.obj", (char*)"assets/texture.bmp");
						temp->getModel()->setCameraPointer(m_pCamera);

						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2 - 5);
						tempNode->SetZPos(j * 2 - 5);
						tempNode->SetScale(0.2f);

						break;
					case 'p':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/giantRobot.obj", (char*)"assets/1pPink.bmp");
						temp->getModel()->setCameraPointer(m_pCamera);
						gameObjectList.push_back(temp);

						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2 - 5);
						tempNode->SetZPos(j * 2 - 5);
						tempNode->SetScale(0.5f);
						nodeHierarchy.push_back(tempNode);
						break;
					case 'E':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/giantRobot.obj", (char*)"assets/1pPink.bmp");
						temp->getModel()->setCameraPointer(m_pCamera);
						gameObjectList.push_back(temp);

						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2 - 5);
						tempNode->SetZPos(j * 2 - 5);
						tempNode->SetScale(0.5f);

						enemyHierarchy.push_back(tempNode);
						break;
					case 's':
						temp = new GameObject(m_pD3DDevice, m_pImmediateContext);
						temp->CreateModel((char*)"assets/PointySphere.obj", (char*)"assets/texture.bmp");
						temp->getModel()->setCameraPointer(m_pCamera);
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i * 2 - 5);
						tempNode->SetZPos(j * 2 - 5);
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

	for (int i = 0; i < nodeHierarchy.size(); i++)
	{
		nodeHierarchy[i]->moveForward(1, m_rootNode);
	}
	for (int i = 0; i < enemyHierarchy.size(); i++)
	{
		enemyHierarchy[i]->lookAt_XZ(cameraX, cameraZ);
		enemyHierarchy[i]->moveForward(1, m_rootNode);
	}
	m_rootNode->execute(world, view, projection, directionLightVector, lightColour, ambientLightColour);
}


void LevelManager::passCameraPos(float x, float z)
{
	cameraX = x;
	cameraZ = z;
}

void LevelManager::setCameraPointer(camera * cam)
{
	m_pCamera = cam;
}
