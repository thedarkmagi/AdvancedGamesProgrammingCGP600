#include "LevelManager.h"



LevelManager::LevelManager()
{
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

		for (int i = 0; i < list.size(); i++)
		{
			for (int j = 0; j < list[i].size(); j++)
			{
				char a= list[i][j];

				switch (list[i][j])
				{
					case 'a':

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
