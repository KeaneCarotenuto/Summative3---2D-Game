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
#include <SFML/Graphics.hpp>
#define Variable(var) GameData::Data(var, #var)
#define Group(var) GameData::DataGroup(var, #var)
#define GetName(var) #var

namespace fs = std::filesystem;
#pragma once
class GameData
{
public:
	struct Data
	{
		std::string DataType;
		std::string DataID;
		std::string DataString;

		Data();
		Data(const Data& copy);
		Data(int Inp, std::string Name)
		{
			DataType = "Int";
			DataID = Name;
			DataString = std::to_string(Inp);
		}
		Data(float Inp, std::string Name)
		{
			DataType = "Float";
			DataID = Name;
			DataString = std::to_string(Inp);
		}
		Data(std::string Inp, std::string Name)
		{
			DataType = "String";
			DataID = Name;
			DataString = Inp;
		}
		Data(bool Inp, std::string Name)
		{
			DataType = "Bool";
			DataID = Name;
			DataString = Inp ? "True" : "False";
		}
		operator int()
		{
			try
			{
				return std::stoi(DataString);
			}
			catch (...)
			{
				return 0;
			}
			
		}
		operator float()
		{
			try
			{
				return std::stof(DataString);
			}
			catch (...)
			{
				return 0;
			}
		}
		operator std::string()
		{
			return DataString;
		}
		operator bool()
		{
			return (DataString == "True");
		}
	};
	struct DataGroup
	{
		std::string GroupID;
		std::vector<Data> m_Data;
		std::vector<DataGroup> m_Groups;
		
		DataGroup();
		DataGroup(std::string _name);

		Data GetDataByID(std::string _ID);
		DataGroup GetGroupByID(std::string _ID);

		template<class T>
		DataGroup(std::vector<T> Inp)
		{
			int index = 1;
			for (T dat : Inp)
			{
				m_Data.push_back(Data(dat,"M" + std::to_string(index++)));
			}
			GroupID = "Vector";
		}

		template<class T>
		DataGroup(std::vector<T> Inp, std::string Name)
		{
			int index = 1;
			for (T dat : Inp)
			{
				m_Data.push_back(Data(dat, "M" + std::to_string(index++)));
			}
			
			GroupID = Name;
		}

		template<class T>
		DataGroup(std::vector<std::vector<T>> Inp, std::string Name)
		{
			int index = 1;
			for (std::vector<T> dat : Inp)
			{
				m_Groups.push_back(DataGroup(dat, "M" + std::to_string(index++)));
			}
			GroupID = Name;
		}

		DataGroup(sf::Vector2f Inp, std::string Name)
		{
			m_Data.push_back(Data(Inp.x, "x"));
			m_Data.push_back(Data(Inp.y, "y"));
			GroupID = Name;
		}

		operator sf::Vector2f()
		{
			sf::Vector2f result;
			if (!m_Data.empty())
			{
				for (Data dat : m_Data)
				{
					if (dat.DataID == "x")
					{
						result.x = dat;
					}
					else if (dat.DataID == "y")
					{
						result.y = dat;
					}

				}
			}
			return result;
		}

		template<class T>
		operator std::vector<T>()
		{
			std::vector<T> result;
			if (!m_Data.empty())
			{
				for (Data dat : m_Data)
				{
					result.push_back(dat);
				}
			}
			return result;
		}
		template<class T>
		operator std::vector<std::vector<T>>()
		{
			std::vector<std::vector<T>> result;
			if (!m_Groups.empty())
			{
				for (DataGroup datg : m_Groups)
				{
					result.push_back(datg);
				}
			}
			return result;
		}

		
		
	};
	

	GameData(std::string Path, std::string _name);
	GameData() 
	{

	}
	void AddVariable(Data dat);
	void AddGroup(DataGroup datg);
	void Save(std::string Path, std::string Filename);
	std::string SaveGroup(DataGroup datg, int depth = 0);
	

	DataGroup FileData;
};
class Loadable
{
public:
	GameData* Data;
	
protected:
	Loadable(std::string Filepath, std::string _name);
	
	std::string FilePath;
};

