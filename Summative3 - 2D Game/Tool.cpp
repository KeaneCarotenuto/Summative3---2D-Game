#include "Tool.h"



Tool::Tool(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability, ToolType _type)
	: CItem(_wind, _pos, _name, "Tools/"), Durability(_durability), Type(_type)
{
	sprite.setScale(5, 5);
	sprite.setPosition(_pos);
}

Tool::Tool(sf::RenderWindow* _wind, GameData::DataGroup datg, ToolType _type)
	: CItem(_wind, datg.GetGroupByID("initialPos"), datg.GroupID, "Tools/"), Durability(datg.GetDataByID("Durability")), Type(_type)
{
	sprite.setScale(5, 5);
	sprite.setPosition(datg.GetGroupByID("initialPos"));
}

void Tool::Draw()
{
	currentInv->draw(sprite);
}


