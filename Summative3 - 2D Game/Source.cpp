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

CPlayer player({ 5000,5000 }, {20,20}, sf::Color::Green);

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "2D Game");
	srand(time(0));
	window.setFramerateLimit(60);
	float spotlightX = 25, spotlightY = 25;
	WorldLayer* world = new WorldLayer();

	sf::View view(sf::FloatRect(0.f, 0.f, 10000.0f, 10000.0f));

	// want to do visibility checks? retrieve the view
	sf::View currentView = window.getView();

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
		world->addPointLight((temp.x/20), (temp.y / 20), 9);
		
		
		
		//<start>Stuff needed for EasySFML
		CObjectController::UpdateObjects();

		view.setCenter(player.rect.getPosition());
		
		window.setView(view);
		
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
