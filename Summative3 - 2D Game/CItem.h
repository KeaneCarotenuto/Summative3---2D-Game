#pragma once
#include <SFML/Graphics.hpp>

#include <iostream>
#include "GameData.h"
#include "EasySFML.h"


class CItem :
	public CGameObject, Loadable
{
public:
	const std::string itemName;

	sf::Texture texture;
	sf::Sprite sprite;

	sf::RenderWindow* currentInv;

	sf::Vector2f initialPos;
	sf::RenderWindow* initialWindow;

	bool operator==(const CItem& _item);

	virtual void FixedUpdate();

protected:
	CItem(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, std::string _filepath);

	

	virtual void Draw();
};

