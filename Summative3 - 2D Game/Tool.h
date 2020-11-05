#pragma once
#include "CItem.h"
class Tool :
    public CItem
{
public:
    Tool(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability);
	Tool(sf::RenderWindow* _wind, GameData::DataGroup datg);
    float Durablity;
	operator GameData::DataGroup()
	{
		GameData::DataGroup datg;

		datg.GroupID = itemName;
		datg.m_Data.push_back(Variable(Durablity));
		datg.m_Groups.push_back(Group(initialPos));

		return datg;
	}

	static CItem* newAxe(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Axe(_wind, _datag); }
	static CItem* newPickaxe(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Pickaxe(_wind, _datag); }
};

class Axe :
    public Tool
{
public:
    Axe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _durability);
	Axe(sf::RenderWindow* _wind, GameData::DataGroup datg);
};

class Pickaxe :
    public Tool
{
public:
    Pickaxe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _durability);
	Pickaxe(sf::RenderWindow* _wind, GameData::DataGroup datg);
};

