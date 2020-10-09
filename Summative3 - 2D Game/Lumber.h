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

	//Functions that create and return an isntance of a specific Object
	static CItem* Stick(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Lumber( LumberType::Stick, _wind, _pos); }
	static CItem* Log(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Lumber( LumberType::Log, _wind, _pos); }
};

