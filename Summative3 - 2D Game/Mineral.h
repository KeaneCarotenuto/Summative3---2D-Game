#pragma once
#include "Resource.h"
#include "GameData.h"

enum class MineralType
{
    Stone,
    CopperOre,
    IronOre
};

class Mineral :
    public Resource
{
public:
    MineralType type;
	
	
	Mineral(MineralType _type, GameData::DataGroup _datag, sf::RenderWindow* _wind, CItem* (*constructor)(sf::RenderWindow* _wind, GameData::DataGroup _datag));
	Mineral(MineralType _type, sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name);


	//Functions that create and return an isntance of a specific Object
	static CItem* Stone(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Mineral(MineralType::Stone, _datag, _wind, Stone); }
	static CItem* CopperOre(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Mineral(MineralType::CopperOre, _datag, _wind, CopperOre); }
	static CItem* IronOre(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Mineral(MineralType::IronOre, _datag, _wind, IronOre); }
	
};

