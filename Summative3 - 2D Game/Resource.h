#pragma once
#include <iostream>

#include "CItem.h"
#include "Stackable.h"

class Resource :
	CItem, Stackable
{
public:
	Resource();

	sf::Font font;

	virtual void Draw();
};

