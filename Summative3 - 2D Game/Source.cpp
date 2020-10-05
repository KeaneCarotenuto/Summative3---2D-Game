#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include <vector>

#include "EasySFML.h"

int FixedUpdate();

void Draw(sf::RenderWindow& mainWindow);

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "2D Game");

	window.setFramerateLimit(60);

	float step = 1 / 60;
	float stepTime = 0;
	bool drawn = false;
	sf::Clock clock;
	while (window.isOpen() == true)
	{
		stepTime += clock.getElapsedTime().asSeconds();
		clock.restart();

		while (stepTime >= step)
		{
			//Main Loop of Game
			if (FixedUpdate() == 0) return 0;

			stepTime -= step;
			drawn = false;
		}

		if (drawn)
		{
			//sf::sleep(sf::seconds(0.01f));
		}
		else
		{
			Draw(window);

			drawn = true;
		}

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
		for each (sf::Drawable * Draw in CWindowUtilities::ToDrawList) //Draw every object on the draw list
		{
			window.draw(*Draw);
		}
		CWindowUtilities::ToDrawList.clear(); //Then empty it so its ready for the next frame
		//<end>
	}

	return 0;
}

int count = 0;
int FixedUpdate()
{
	//std::cout << count << std::endl;
	count++;

	return 1;
}

void Draw(sf::RenderWindow& mainWindow)
{
	
}
