////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	SpecialTile
//--------------------------------------------------------//
//					BSE20021
//    Author    :	Nerys Thamm
//					Keane Carotenuto
//--------------------------------------------------------//
//    E-mails   :	NerysThamm@gmail.com
//					Keane Carotenuto@gmail.com
//========================================================//
////////////////////////////////////////////////////////////

#include <SFML/Graphics.hpp>
#include "EasySFML.h"
#include "Resource.h"
#pragma once

enum class SpecialType
{
	Tree,
	Boulder
};

class SpecialTile
	: CGameObject
{
public:
	int health;
	SpecialType type;
	sf::Sprite Sprite;
	sf::Texture Texture;
	std::vector<Resource> Droptable;
	sf::Vector2f Pos;
	SpecialTile(SpecialType _type, sf::Vector2f _pos, std::string _texturepath);
	
	
	//Todo: Make interface class for Special tiles such as resource nodes
};

class Tree
	: public SpecialTile
{
public:
	Tree(sf::Vector2f _pos);
};

class Boulder
	: public SpecialTile
{
public:
	Boulder(sf::Vector2f _pos);
};
