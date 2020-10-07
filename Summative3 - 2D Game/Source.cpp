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
	sf::RenderWindow window(sf::VideoMode(5000, 5000), "2D Game");
	srand(time(0));
	window.setFramerateLimit(60);
	float spotlightX = 25, spotlightY = 25;
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
		sf::Vector2f temp = player.rect.getPosition();
		world->addPointLight((temp.x/100)*20, (temp.y / 100)*20, 9);
		
		
		
		//<start>Stuff needed for EasySFML
		CObjectController::UpdateObjects();
		
		window.clear();
		world->renderLightMap(true);
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
