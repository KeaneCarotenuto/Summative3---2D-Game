#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>

#include "EasySFML.h"
#include "WorldLayer.h"

#include "CPlayer.h"

CPlayer player({ 100,100 }, {20,30}, sf::Color::Green);

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "2D Game");
	srand(time(0));
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

		world->resetLightMap();
		world->addPointLight(10, 10, 10);
		world->addPointLight(20, 20, 9);
		world->addPointLight(30, 30, 10);
		world->addPointLight(40, 40, 9);
		world->addPointLight(50, 50, 10);
		world->addPointLight(0, 0, 9);
		//<start>Stuff needed for EasySFML
		CObjectController::UpdateObjects();
		
		window.clear();
		world->renderLightMap();
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
