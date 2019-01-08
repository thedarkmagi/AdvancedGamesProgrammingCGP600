#include "LevelManager.h"



LevelManager::LevelManager(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	m_pD3DDevice = device;
	m_pImmediateContext = deviceContext;
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

		m_rootNode = new SceneNode();

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
						gameObjectList.push_back(temp);
						//tempNode->SetModel(temp->getModel());#
						tempNode->SetGameObject(temp);
						m_rootNode->addChildNode(tempNode);
						tempNode->SetXPos(i*2);
						tempNode->SetZPos(j*2);
						break;
					case 'b':

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
	/*for (int i = 0; i < gameObjectList.size(); i++)
	{
		gameObjectList[i]->setLightingValues(directionLightVector, lightColour, ambientLightColour);
		gameObjectList[i]->update(world, view, projection);
	}*/
	m_rootNode->execute(world, view, projection, directionLightVector, lightColour, ambientLightColour);
}

void LevelManager::update(XMMATRIX * world, XMMATRIX * view, XMMATRIX * projection)
{
	
}
