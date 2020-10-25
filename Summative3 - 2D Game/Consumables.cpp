#include "Consumables.h"

Consumables::Consumables(ConsumableType _type, GameData::DataGroup _datag, sf::RenderWindow* _wind, CItem* (*constructor)(sf::RenderWindow* _wind, GameData::DataGroup _datag)):
	type(_type), Resource(_wind, _datag, constructor)
{
}

Consumables::Consumables(ConsumableType _type, sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name):
	type(_type), Resource(_wind, _pos, _name)
{
}
