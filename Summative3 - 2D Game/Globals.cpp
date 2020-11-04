#include "Globals.h"

std::map <std::string, sf::RenderWindow*> Globals::mapOfWindows;
std::vector<sf::RenderWindow*> Globals::inventories;
int Globals::seed = 1234;

void Globals::RegisterWindow(std::string _str, sf::RenderWindow* _wind)
{
	inventories.push_back(_wind);

	mapOfWindows[_str] = _wind;
}
