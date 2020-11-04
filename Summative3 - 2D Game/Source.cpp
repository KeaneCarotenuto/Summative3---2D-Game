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
#include "Globals.h"

void GenNewIsland(ItemManager* itemMngr);

void CheckPlayerHitsEdge(CPlayer& player, ItemManager* itemMngr);

void Drawing(sf::View& view, CPlayer& player);

void CreateEntities(CPlayer& player, ItemManager* itemMngr);

void CreateWindows();

int StartGame();

int GameLoop(ItemManager* itemMngr, CPlayer& player);

int main() {
	StartGame();

	return 0;
}

int StartGame()
{
	srand(Globals::seed);

	CreateWindows();

	sf::RenderWindow* worldInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator worldInvIt = Globals::mapOfWindows.find("WorldInv");
	if (worldInvIt != Globals::mapOfWindows.end()) {
		worldInv = (*worldInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find WorldInv Window";
	}

	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator playerInvIt = Globals::mapOfWindows.find("PlayerInv");
	if (playerInvIt != Globals::mapOfWindows.end()) {
		playerInv = (*playerInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find PlayerInv Window";
	}

	WorldLayer::currentWorld = new WorldLayer(Globals::seed);

	//Create itemmanager and hand it the map of windows
	ItemManager* itemMngr = new ItemManager();

	//Create Player
	CPlayer player({ 0,0 }, { 20,20 }, sf::Color::Green);
	player.rect.setPosition(WorldLayer::currentWorld->GetFirstSandTilePos());

	CreateEntities(player, itemMngr);

	float spotlightX = 25, spotlightY = 25;

	

	int gameLoopReturn = GameLoop(itemMngr, player);
	if (gameLoopReturn == 0) return 0;
}

int GameLoop(ItemManager* itemMngr, CPlayer& player)
{
	sf::View view(sf::FloatRect(0.f, 0.f, 1000.0f, 1000.0f));

	sf::RenderWindow* worldInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator worldInvIt = Globals::mapOfWindows.find("WorldInv");
	if (worldInvIt != Globals::mapOfWindows.end()) {
		worldInv = (*worldInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find WorldInv Window";
	}

	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator playerInvIt = Globals::mapOfWindows.find("PlayerInv");
	if (playerInvIt != Globals::mapOfWindows.end()) {
		playerInv = (*playerInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find PlayerInv Window";
	}

	while (worldInv->isOpen() == true)
	{
		sf::Event newEvent;

		while (worldInv->pollEvent(newEvent))
		{
			if (newEvent.type == sf::Event::Closed)
			{
				delete itemMngr;
				worldInv->close();
				return 0;
			}

			if (newEvent.type == sf::Event::MouseWheelMoved)
			{
				if (newEvent.mouseWheel.delta > 0) {
					view.zoom(0.75f);
				}
				else {
					view.zoom(1 / 0.75f);
				}

				view.setSize(std::clamp(std::ceil(view.getSize().x / 100) * 100, 100.0f, 1500.0f), std::clamp(std::ceil(view.getSize().y / 100) * 100, 100.0f, 1500.0f));
				std::cout << view.getSize().x << std::endl;
			}

		}

		//Keeps Invntory on Top
		HWND hwnd = playerInv->getSystemHandle();
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


		/*world->resetLightMap();
		sf::Vector2f temp = player.rect.getPosition();
		world->addPointLight((int)(temp.x/20), (int)(temp.y / 20), 9);*/


		Drawing(view, player);


		CheckPlayerHitsEdge(player, itemMngr);
	}

	return 1;
}

void CreateWindows()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1000, 1000), "2D Game");
	sf::RenderWindow* inventory = new sf::RenderWindow(sf::VideoMode(200, 500), "Inventory");
	sf::RenderWindow* crafting = new sf::RenderWindow(sf::VideoMode(200, 200), "Crafting");

	inventory->setPosition(sf::Vector2i(window->getPosition().x - inventory->getSize().x, window->getPosition().y));
	crafting->setPosition(sf::Vector2i(inventory->getPosition().x, inventory->getPosition().y + inventory->getSize().y));

	//window.setVerticalSyncEnabled(true);
	window->setFramerateLimit(60);

	//Create Map of Widows and thier ID's
	std::map <std::string, sf::RenderWindow*> windowsMap{
		{ "PlayerInv",inventory },
		{ "WorldInv",window },
		{ "CraftingInv",crafting }
	};

	Globals::mapOfWindows = windowsMap;

	for (std::map < std::string, sf::RenderWindow*>::iterator it = windowsMap.begin(); it != windowsMap.end(); it++) {
		Globals::RegisterWindow(it->first, it->second);
	}
}

void CreateEntities(CPlayer& player, ItemManager* itemMngr)
{
	CEntity* bird = new CEntity(EntityType::Bird, { 1000,1000 }, { 15,15 }, sf::Color::White);
	bird->player = &player;
	bird->itemManager = itemMngr;
	itemMngr->entities.push_back(bird);

	CEntity* fish = new CEntity(EntityType::Fish, { 100,100 }, { 15,15 }, sf::Color::Red);
	fish->player = &player;
	fish->itemManager = itemMngr;
	itemMngr->entities.push_back(fish);
}

void Drawing(sf::View& view, CPlayer& player)
{
	sf::RenderWindow* worldInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator worldInvIt = Globals::mapOfWindows.find("WorldInv");
	if (worldInvIt != Globals::mapOfWindows.end()) {
		worldInv = (*worldInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find WorldInv Window";
	}

	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator playerInvIt = Globals::mapOfWindows.find("PlayerInv");
	if (playerInvIt != Globals::mapOfWindows.end()) {
		playerInv = (*playerInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find PlayerInv Window";
	}

	sf::RenderWindow* craftingInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator craftingInvIt = Globals::mapOfWindows.find("CraftingInv");
	if (craftingInvIt != Globals::mapOfWindows.end()) {
		craftingInv = (*craftingInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find CraftingInv Window";
	}

	playerInv->clear();
	worldInv->clear();
	craftingInv->clear();

	worldInv->draw(*WorldLayer::currentWorld);

	CObjectController::UpdateObjects();

	playerInv->display();
	craftingInv->display();

	view.setCenter(player.rect.getPosition() + sf::Vector2f(player.rect.getSize().x / 2, player.rect.getSize().y / 2));

	worldInv->setView(view);

	WorldLayer::currentWorld->renderTileMaps();
	//world->renderLightMap();

	WorldLayer::currentWorld->DrawSpecial();

	for (sf::Drawable* Draw : CWindowUtilities::ToDrawList) //Draw every object on the draw list
	{
		worldInv->draw(*Draw);
	}
	worldInv->display();
	CWindowUtilities::ToDrawList.clear(); //Then empty it so its ready for the next frame
}

void CheckPlayerHitsEdge(CPlayer& player, ItemManager* itemMngr)
{
	bool changedWorld = false;
	if (player.rect.getPosition().x < 0) {
		Globals::seed -= 1;
		player.rect.setPosition(10000 - 50, player.rect.getPosition().y);

		GenNewIsland(itemMngr);
		changedWorld = true;
	}

	if (player.rect.getPosition().x > 10000) {
		Globals::seed += 1;
		player.rect.setPosition(50, player.rect.getPosition().y);

		GenNewIsland(itemMngr);
		changedWorld = true;
	}

	if (player.rect.getPosition().y < 0) {
		Globals::seed -= 10000;
		player.rect.setPosition(player.rect.getPosition().x, 10000 - 50);

		GenNewIsland(itemMngr);
		changedWorld = true;
	}

	if (player.rect.getPosition().y > 10000) {
		Globals::seed += 10000;
		player.rect.setPosition(player.rect.getPosition().x, 50);

		GenNewIsland(itemMngr);
		changedWorld = true;
	}
}

void GenNewIsland(ItemManager* itemMngr)
{
	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator invWndIt = Globals::mapOfWindows.find("PlayerInv");
	if (invWndIt != Globals::mapOfWindows.end()) {
		playerInv = (*invWndIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find PlayerInv Window";
	}

	srand(Globals::seed);
	delete WorldLayer::currentWorld;
	WorldLayer::currentWorld = new WorldLayer(Globals::seed);

	for (CItem* _item : itemMngr->items) {
		if (_item->currentInv != playerInv) {
			std::vector<CItem*>::iterator pos = std::find(itemMngr->items.begin(), itemMngr->items.end(), _item);
			if (pos != itemMngr->items.end()) {
				delete _item;
				itemMngr->items.erase(pos);
			}
		}
	}
}

