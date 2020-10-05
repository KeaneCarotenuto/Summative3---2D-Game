#pragma once
#include <SFML/Graphics.hpp>

#include "EasySFML.h"


class CPlayer:
	private CGameObject
{
public:
	sf::RectangleShape rect;
	float moveSpeed = 5;
	
	CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col);

	void Movement();
	void ScreenWrap();

	virtual void FixedUpdate();
};

