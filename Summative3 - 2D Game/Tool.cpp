#include "Tool.h"



Tool::Tool(ToolType _type, sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability)
	: CItem(_wind, _pos, _name, "Tools/"), Durability(_durability), type(_type)
{
	sprite.setScale(5, 5);
	sprite.setPosition(_pos);
	Font.loadFromFile("Resources/Fonts/uni.ttf");
	DurabilityText.setFont(Font);
	DurabilityText.setScale(sf::Vector2f(0.5, 0.5));
	DurabilityText.setOrigin(sf::Vector2f(0, -200));
}

Tool::Tool(sf::RenderWindow* _wind, GameData::DataGroup datg, ToolType _type)
	: CItem(_wind, datg.GetGroupByID("initialPos"), datg.GroupID, "Tools/"), Durability(datg.GetDataByID("Durability")), type(_type)
{
	sprite.setScale(5, 5);
	sprite.setPosition(datg.GetGroupByID("initialPos"));
	Font.loadFromFile("Resources/Fonts/uni.ttf");
	DurabilityText.setFont(Font);
	DurabilityText.setScale(sf::Vector2f(0.5, 0.5));
	DurabilityText.setOrigin(sf::Vector2f(0, -200));
}

void Tool::Draw()
{
	DurabilityText.setString("Dur: " + std::to_string((int)Durability));
	DurabilityText.setFillColor((Durability < 20 ? sf::Color::Red : sf::Color::White));
	DurabilityText.setPosition(sprite.getPosition());
	currentInv->draw(sprite);
	currentInv->draw(DurabilityText);
}


