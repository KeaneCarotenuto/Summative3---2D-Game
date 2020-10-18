#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "EasySFML.h"
#include "CPlayer.h"
#include "CItem.h"
#include "Stackable.h"
#include "Lumber.h"

class ItemFactory
{
	std::map <std::string, CItem* (*)(sf::RenderWindow* _wind, sf::Vector2f _pos)> mapOfItems = {
		{"Stick", &Lumber::Stick},
		{"Log", &Lumber::Log}
	};

};

