#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include <vector>

#include "EasySFML.h"
#include "WorldLayer.h"

#include "CPlayer.h"

CPlayer player({ 100,100 }, {20,30}, sf::Color::Green);

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "2D Game");

	window.setFramerateLimit(60);

	WorldLayer* world = new WorldLayer();

	while (window.isOpen() == true)
	{
		sf::Event newEvent;

		while (window.pollEvent(newEvent))
		{
			if (newEvent.type == sf::Event::Closed)
			{
				window.close();
			}
		}


		//<start>Stuff needed for EasySFML
		CObjectController::UpdateObjects();
		
		window.clear();
		window.draw(*world);
		for (sf::Drawable * Draw : CWindowUtilities::ToDrawList) //Draw every object on the draw list
		{
			window.draw(*Draw);
		}
		window.display();
		CWindowUtilities::ToDrawList.clear(); //Then empty it so its ready for the next frame
		//<end>
	}
}
