#include "Tool.h"



Tool::Tool(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability)
	: CItem(_wind, _pos, _name, "Tools/")
{
}

Axe::Axe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _dur)
	: Tool(_wind, _pos, "Axe", _dur)
{
}

Pickaxe::Pickaxe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _dur)
	: Tool(_wind, _pos, "Pickaxe", _dur)
{
}
