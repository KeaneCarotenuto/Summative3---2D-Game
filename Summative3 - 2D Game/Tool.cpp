#include "Tool.h"



Tool::Tool(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability)
	: CItem(_wind, _pos, _name, "Tools/"), Durablity(_durability)
{
	sprite.setScale(5, 5);
	sprite.setPosition(_pos);
}

Tool::Tool(sf::RenderWindow* _wind, GameData::DataGroup datg)
	: CItem(_wind, datg.GetGroupByID("initialPos"), datg.GroupID, "Tools/"), Durablity(datg.GetDataByID("Durability"))
{
	sprite.setScale(5, 5);
	sprite.setPosition(datg.GetGroupByID("initialPos"));
}

Axe::Axe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _dur)
	: Tool(_wind, _pos, "Axe", _dur)
{
}

Axe::Axe(sf::RenderWindow* _wind, GameData::DataGroup datg)
	: Tool(_wind, datg)
{
}

Pickaxe::Pickaxe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _dur)
	: Tool(_wind, _pos, "Pickaxe", _dur)
{
}

Pickaxe::Pickaxe(sf::RenderWindow* _wind, GameData::DataGroup datg)
	: Tool(_wind, datg)
{
}
