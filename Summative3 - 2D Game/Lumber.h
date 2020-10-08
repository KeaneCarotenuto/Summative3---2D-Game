#pragma once

#include "Resource.h"

enum class LumberType
{
	Stick,
	Log
};

class Lumber:
	public Resource
{
public:
	LumberType type;


	Lumber(LumberType _type, sf::RenderWindow* _wind, sf::Vector2f _pos);

};

