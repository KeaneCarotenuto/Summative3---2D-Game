#include <SFML/Graphics.hpp>
#include "EasySFML.h"
#include "Resource.h"
#pragma once

class SpecialTile
	: CGameObject
{
public:
	sf::Sprite Sprite;
	sf::Texture Texture;
	std::vector<Resource> Droptable;
	sf::Vector2f Pos;
	SpecialTile(sf::Vector2f _pos, std::string _texturepath);
	
	
	//Todo: Make interface class for Special tiles such as resource nodes
};

class Tree
	: public SpecialTile
{
public:
	Tree(sf::Vector2f _pos);
};

