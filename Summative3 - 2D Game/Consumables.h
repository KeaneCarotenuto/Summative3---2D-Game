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

    static CItem* Water(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Consumables(ConsumableType::Water, _datag, _wind, Water); }
    static CItem* Berries(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Consumables(ConsumableType::Berries, _datag, _wind, Berries); }
    static CItem* Meat(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Consumables(ConsumableType::Meat, _datag, _wind, Meat); }
};

