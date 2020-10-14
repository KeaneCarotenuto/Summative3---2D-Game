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
#define Variable(var) GameData::Data(var, #var)
#define Group(var) GameData::DataGroup(var, #var)

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
		//Data(int Inp)
		//{
		//	DataType = "Int";
		//	DataID = GET_VARIABLE_NAME(Inp);
		//	DataString = std::to_string(Inp);
		//}
		//Data(float Inp)
		//{
		//	DataType = "Float";
		//	DataID = GET_VARIABLE_NAME(Inp);
		//	DataString = std::to_string(Inp);
		//}
		//Data(std::string Inp)
		//{
		//	DataType = "String";
		//	DataID = GET_VARIABLE_NAME(Inp);
		//	DataString = Inp;
		//}
		//Data(bool Inp)
		//{
		//	DataType = "Bool";
		//	DataID = GET_VARIABLE_NAME(Inp);
		//	DataString = Inp ? "True" : "False";
		//}
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
			return std::stoi(DataString);
		}
		operator float()
		{
			return std::stof(DataString);
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
		DataGroup(std::vector<std::vector<T>> Inp)
		{
			int index = 1;
			for (T dat : Inp)
			{
				m_Groups.push_back(DataGroup(dat, "V" + std::to_string(index)));
			}
			GroupID = "Vector(Vector)";
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
		
		
	};
	

	GameData(std::string Path, std::string _name);
	GameData() 
	{

	}
	void Save(std::string Path, std::string Filename);
	std::string SaveGroup(DataGroup datg, int depth = 0);
	std::string GetByID(std::string ID, std::string GroupID = "");

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

