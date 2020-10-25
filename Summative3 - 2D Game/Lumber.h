#pragma once
#include "GameData.h"
#include "Resource.h"

enum class LumberType
{
	Stick,
	Log
};

class Lumber:
	public Resource
{
public:
	LumberType type;


	
	Lumber(LumberType _type, GameData::DataGroup _datag, sf::RenderWindow* _wind, CItem* (*constructor)(sf::RenderWindow* _wind, GameData::DataGroup _datag));
	Lumber(LumberType _type, sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name);

	//Functions that create and return an isntance of a specific Object

	static CItem* Stick(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Lumber(LumberType::Stick, _datag, _wind, Stick); }
	static CItem* Log(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Lumber(LumberType::Log, _datag, _wind, Log); }
};

