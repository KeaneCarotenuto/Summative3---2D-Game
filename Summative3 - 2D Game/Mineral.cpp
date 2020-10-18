#include "Mineral.h"



Mineral::Mineral(MineralType _type, GameData::DataGroup _datag, sf::RenderWindow* _wind, CItem* (*constructor)(sf::RenderWindow* _wind, GameData::DataGroup _datag)):
	type(_type), Resource(_wind, _datag, constructor)
{

}
