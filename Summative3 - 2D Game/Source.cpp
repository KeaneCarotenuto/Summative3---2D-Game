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
#include "CItem.h"

CPlayer player({ 10,10 }, {20,20}, sf::Color::Green);

int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "2D Game");
	sf::RenderWindow inventory(sf::VideoMode(200, 500), "2D Game");

	srand(time(0));
	window.setFramerateLimit(60);
	float spotlightX = 25, spotlightY = 25;
	WorldLayer* world = new WorldLayer();

	CItem* testItem = new CItem(ItemType::Stick, &window, &inventory, true);
	CItem* testItem2 = new CItem(ItemType::Log, &window, &inventory, false);
	

	sf::View view(sf::FloatRect(0.f, 0.f, 1000.0f, 1000.0f));

	while (window.isOpen() == true)
	{
		sf::Event newEvent;

		while (window.pollEvent(newEvent))
		{
			if (newEvent.type == sf::Event::Closed)
			{
				window.close();
			}

			if (newEvent.type == sf::Event::MouseWheelMoved)
			{
				if (newEvent.mouseWheel.delta > 0) {
					view.zoom(0.75f);
				}
				else {
					view.zoom(1/0.75f);
				}
				view.setSize(std::ceil(view.getSize().x / 100) * 100, std::ceil(view.getSize().y/ 100) * 100);
				std::cout << view.getSize().x << std::endl;
			}
			
		}

		world->resetLightMap();
		sf::Vector2f temp = player.rect.getPosition();
		world->addPointLight((temp.x/20), (temp.y / 20), 9);
		
		
		inventory.clear();
		window.clear();
		//<start>Stuff needed for EasySFML

		window.draw(*world);

		CObjectController::UpdateObjects();

		inventory.display();

		view.setCenter(player.rect.getPosition() + sf::Vector2f(player.rect.getSize().x / 2, player.rect.getSize().y / 2));
		
		window.setView(view);
		
		//world->renderLightMap();
		
		for (sf::Drawable * Draw : CWindowUtilities::ToDrawList) //Draw every object on the draw list
		{
			window.draw(*Draw);
		}		
		window.display();
		CWindowUtilities::ToDrawList.clear(); //Then empty it so its ready for the next frame
		//<end>

		
		
	}
}
