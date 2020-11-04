#pragma once
#include <SFML/Graphics.hpp>
#include "WorldLayer.h"

class Globals
{
public:

	static std::map <std::string, sf::RenderWindow*> mapOfWindows;
	static std::vector<sf::RenderWindow*> inventories;
	static WorldLayer* currentWorld;
	static int seed;

	static void RegisterWindow(std::string _str, sf::RenderWindow* _wind);
};