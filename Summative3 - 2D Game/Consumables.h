////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	Consumables
//                  Base class for consumables
//--------------------------------------------------------//
//					BSE20021
//    Author    :	Nerys Thamm
//					Keane Carotenuto
//--------------------------------------------------------//
//    E-mails   :	NerysThamm@gmail.com
//					Keane Carotenuto@gmail.com
//========================================================//
////////////////////////////////////////////////////////////

#pragma once
#include "Resource.h"
#include "GameData.h"

enum class ConsumableType
{
    Water,
    Berries,
    Meat
};
class Consumables :
    public Resource
{
public:
    ConsumableType type;
    Consumables(ConsumableType _type, GameData::DataGroup _datag, sf::RenderWindow* _wind, CItem* (*constructor)(sf::RenderWindow* _wind, GameData::DataGroup _datag));
    Consumables(ConsumableType _type, sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name);

    static CItem* Water(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Consumables(ConsumableType::Water, _datag, _wind, Water); }
    static CItem* Berries(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Consumables(ConsumableType::Berries, _datag, _wind, Berries); }
    static CItem* Meat(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Consumables(ConsumableType::Meat, _datag, _wind, Meat); }
};

