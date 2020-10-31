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
#include "CEntity.h"
#include "CItem.h"
#include "ItemManager.h"
#include "Lumber.h"

void GenNewIsland(int seed, WorldLayer*& world, ItemManager* itemMngr, sf::RenderWindow* _inv);

int main() {
	int seed = 1234;
	srand(seed);

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "2D Game");
	sf::RenderWindow inventory(sf::VideoMode(200, 500), "Inventory");
	sf::RenderWindow crafting(sf::VideoMode(200, 200), "Crafting");

	inventory.setPosition(sf::Vector2i(window.getPosition().x - inventory.getSize().x, window.getPosition().y));
	crafting.setPosition(sf::Vector2i(inventory.getPosition().x, inventory.getPosition().y + inventory.getSize().y));

	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);
	float spotlightX = 25, spotlightY = 25;


	//Create Map of Widows and thier ID's
	std::map <std::string, sf::RenderWindow*> windowsMap{
		{"PlayerInv",&inventory},
		{"WorldInv",&window},
		{"CraftingInv",&crafting}
	};

	//Create itemmanager and hand it the map of windows
	ItemManager* itemMngr = new ItemManager(windowsMap);
	
	WorldLayer* world = new WorldLayer(seed);
	itemMngr->world = world;
	CPlayer player({ 0,0 }, { 20,20 }, sf::Color::Green, world);
	CEntity* bird = new CEntity(EntityType::Bird, { 1000,1000 }, { 15,15 }, sf::Color::White, world);
	bird->player = &player;
	bird->itemManager = itemMngr;
	itemMngr->entities.push_back(bird);

	CEntity* fish = new CEntity(EntityType::Fish, { 100,100 }, { 15,15 }, sf::Color::Red, world);
	fish->player = &player;
	fish->itemManager = itemMngr;
	itemMngr->entities.push_back(fish);

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
		crafting.clear();
		//<start>Stuff needed for EasySFML

		window.draw(*world);

		CObjectController::UpdateObjects();

		inventory.display();
		crafting.display();

		view.setCenter(player.rect.getPosition() + sf::Vector2f(player.rect.getSize().x / 2, player.rect.getSize().y / 2));
		
		window.setView(view);
		
		world->renderTileMaps();
		//world->renderLightMap();

		world->DrawSpecial();
		
		for (sf::Drawable * Draw : CWindowUtilities::ToDrawList) //Draw every object on the draw list
		{
			window.draw(*Draw);
		}		
		window.display();
		CWindowUtilities::ToDrawList.clear(); //Then empty it so its ready for the next frame
		//<end>

		itemMngr->LateDelete();
		

		if (player.rect.getPosition().x < 0) {
			seed -= 1;
			player.rect.setPosition(10000 - 50, player.rect.getPosition().y);

			GenNewIsland(seed, world, itemMngr, &inventory);
		}

		if (player.rect.getPosition().x > 10000) {
			seed += 1;
			player.rect.setPosition(50, player.rect.getPosition().y);

			GenNewIsland(seed, world, itemMngr, &inventory);
		}

		if (player.rect.getPosition().y < 0) {
			seed -= 10000;
			player.rect.setPosition(player.rect.getPosition().x, 10000 - 50);

			GenNewIsland(seed, world, itemMngr, &inventory);
		}

		if (player.rect.getPosition().y > 10000) {
			seed += 10000;
			player.rect.setPosition(player.rect.getPosition().x, 50);

			GenNewIsland(seed, world, itemMngr, &inventory);
		}
	}
}

void GenNewIsland(int seed, WorldLayer*& world, ItemManager* itemMngr, sf::RenderWindow* _inv)
{
	srand(seed);
	delete world;
	world = new WorldLayer(seed);

	for (CItem* _item : itemMngr->items) {
		if (_item->currentInv != _inv) {
			std::vector<CItem*>::iterator pos = std::find(itemMngr->items.begin(), itemMngr->items.end(), _item);
			if (pos != itemMngr->items.end()) {
				delete _item;
				itemMngr->items.erase(pos);
			}
		}
	}
}

