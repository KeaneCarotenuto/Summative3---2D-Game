#pragma once
#include <SFML/Graphics.hpp>

class Globals
{
public:

	static std::map <std::string, sf::RenderWindow*> mapOfWindows;
	static std::vector<sf::RenderWindow*> inventories;

	static void RegisterWindow(std::string _str, sf::RenderWindow* _wind);
};

