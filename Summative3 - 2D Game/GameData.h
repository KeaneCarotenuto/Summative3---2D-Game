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


////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :   GameData.h
//   Handles file interfacing and data serialisation
//--------------------------------------------------------//
//                    BSE20021
//    Author    :    Nerys Thamm
//                    Keane Carotenuto
//--------------------------------------------------------//
//    E-mails   :    NerysThamm@gmail.com
//                    Keane Carotenuto@gmail.com
//========================================================//
////////////////////////////////////////////////////////////

namespace fs = std::filesystem;
#pragma once
class GameData
{
public:
	struct Data //Class representing Serialised data
	{
		/// <summary>
		/// Type, Name, and Contents of data
		/// </summary>
		std::string DataType;
		std::string DataID;
		std::string DataString;

		/// <summary>
		/// Constructor
		/// </summary>
		Data();
		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="copy"></param>
		Data(const Data& copy);
		/// <summary>
		/// Conversion constructors
		/// </summary>
		/// <param name="Inp"></param>
		/// <param name="Name"></param>
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
		/// <summary>
		/// Conversion Operators
		/// </summary>
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
	struct DataGroup //Container for data
	{
		/// <summary>
		/// Group ID, vector of data, vector of child groups
		/// </summary>
		std::string GroupID;
		std::vector<Data> m_Data;
		std::vector<DataGroup> m_Groups;
		
		/// <summary>
		/// Constructors
		/// </summary>
		DataGroup();
		DataGroup(std::string _name);

		/// <summary>
		/// Get Methods
		/// </summary>
		/// <param name="_ID"></param>
		/// <returns></returns>
		Data GetDataByID(std::string _ID);
		DataGroup GetGroupByID(std::string _ID);

		/// <summary>
		/// Conversion constructor for vectors
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="Inp"></param>
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

		/// <summary>
		/// Conversion constructor for vectors
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="Inp"></param>
		/// <param name="Name"></param>
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

		/// <summary>
		/// Conversion constructor for nexted vectors
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="Inp"></param>
		/// <param name="Name"></param>
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

		/// <summary>
		/// Conversion constructor for Vector2f
		/// </summary>
		/// <param name="Inp"></param>
		/// <param name="Name"></param>
		DataGroup(sf::Vector2f Inp, std::string Name)
		{
			m_Data.push_back(Data(Inp.x, "x"));
			m_Data.push_back(Data(Inp.y, "y"));
			GroupID = Name;
		}

		/// <summary>
		/// Conversion operator for vector2f
		/// </summary>
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

		/// <summary>
		/// Conversion operator for Vector
		/// </summary>
		/// <typeparam name="T"></typeparam>
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
		/// <summary>
		/// Conversion operator for nested vectors
		/// </summary>
		/// <typeparam name="T"></typeparam>
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
	
	/// <summary>
	/// Constructors
	/// </summary>
	/// <param name="Path"></param>
	/// <param name="_name"></param>
	GameData(std::string Path, std::string _name);
	GameData() 
	{

	}
	/// <summary>
	/// Serialisation methods
	/// </summary>
	/// <param name="dat"></param>
	void AddVariable(Data dat);
	void AddGroup(DataGroup datg);
	void Save(std::string Path, std::string Filename);
	std::string SaveGroup(DataGroup datg, int depth = 0);
	

	DataGroup FileData;
};
class Loadable
{
public:
	/// <summary>
	/// Stored data for instance
	/// </summary>
	GameData* Data;
	
protected:
	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="Filepath"></param>
	/// <param name="_name"></param>
	Loadable(std::string Filepath, std::string _name);
	/// <summary>
	/// Path of config file
	/// </summary>
	std::string FilePath;
};

