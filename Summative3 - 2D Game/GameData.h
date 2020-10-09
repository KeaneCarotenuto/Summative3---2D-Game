#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <ctime>
#include <time.h>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;
#pragma once
class GameData
{
public:
	struct Data
	{
		std::string DataID;
		std::string DataString;
	};
	struct DataGroup
	{
		std::string GroupID;
		int linecount;
		std::vector<Data> Data;
		
	};
	

	GameData(std::string Path, std::string _name);
	std::string GetByID(std::string ID, std::string GroupID = "");

	std::vector<DataGroup> FileData;
};
class Loadable
{
public:
	GameData* Data;
protected:
	Loadable(std::string Filepath, std::string _name);
	
	std::string FilePath;
};

