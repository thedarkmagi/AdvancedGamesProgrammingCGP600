#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	void ReadFromFile(string input);

private:
	//string list[];
	vector<string> list;
};

