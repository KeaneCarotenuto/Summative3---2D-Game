#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>

#include "EasySFML.h"

enum class ItemType
{
	Stick,
	Log,
	Pebbles,
	Rock
};

class CItem :
	public CGameObject
{
public:
	sf::Sprite sprite;
	sf::Texture texture;
	ItemType type;

	sf::RenderWindow* currentInv;

	
	sf::Vector2f initialPos;
	sf::RenderWindow* initialWindow;

	CItem();
	//CItem(ItemType _type, sf::RenderWindow* _wind, sf::Vector2f _pos);

	virtual void FixedUpdate();

protected:

	virtual void Draw();
};

