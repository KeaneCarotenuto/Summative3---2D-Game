#pragma once
#include <SFML/Graphics.hpp>

#include "EasySFML.h"

#include <iostream>;

enum class ItemType
{
	Stick,
	Log,
	Pebbles,
	Rock
};

class CItem :
	private CGameObject
{
public:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::VertexArray vertices;
	ItemType type;

	bool isInInv = false;
	bool beingDragged = false;

	sf::RenderWindow* main;
	sf::RenderWindow* inv;
	

	CItem(ItemType _type, sf::RenderWindow* _main, sf::RenderWindow* _inv, bool _isInInv);

	virtual void FixedUpdate();

private:
	sf::Vector2f initialPos;
};

