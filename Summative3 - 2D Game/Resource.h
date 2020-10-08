#pragma once
#include <iostream>

#include "CItem.h"
#include "Stackable.h"

class Resource :
	public CItem, public Stackable
{
public:
	Resource(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name);

	virtual void Draw();
};

