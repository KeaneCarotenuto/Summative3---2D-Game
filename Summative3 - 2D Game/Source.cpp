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
#include "ItemManager.h"
#include "Lumber.h"




int main() {
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "2D Game");
	sf::RenderWindow inventory(sf::VideoMode(200, 500), "Inventory");

	srand((int)time(0));
	window.setFramerateLimit(60);
	float spotlightX = 25, spotlightY = 25;


	//Create Map of Widows and thier ID's
	std::map <std::string, sf::RenderWindow*> windowsMap{
		{"PlayerInv",&inventory},
		{"WorldInv",&window}
	};

	//Create itemmanager and hand it the map of windows
	ItemManager* itemMngr = new ItemManager(windowsMap);
	
	WorldLayer* world = new WorldLayer();
	CPlayer player({ 10,10 }, { 20,20 }, sf::Color::Green, world);
	player.rect.setPosition(world->GetFirstSandTilePos());

	sf::View view(sf::FloatRect(0.f, 0.f, 1000.0f, 1000.0f));

	while (window.isOpen() == true)
	{
		sf::Event newEvent;

		while (window.pollEvent(newEvent))
		{
			if (newEvent.type == sf::Event::Closed)
			{
				delete itemMngr;
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

		//Keeps Invntory on Top
		HWND hwnd = inventory.getSystemHandle();
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


		/*world->resetLightMap();
		sf::Vector2f temp = player.rect.getPosition();
		world->addPointLight((int)(temp.x/20), (int)(temp.y / 20), 9);*/
		
		
		inventory.clear();
		window.clear();
		//<start>Stuff needed for EasySFML

		window.draw(*world);

		CObjectController::UpdateObjects();

		inventory.display();

		view.setCenter(player.rect.getPosition() + sf::Vector2f(player.rect.getSize().x / 2, player.rect.getSize().y / 2));
		
		window.setView(view);
		
		world->renderTileMaps();
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
