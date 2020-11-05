#include <SFML/Graphics.hpp>

#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include <vector>
#include <time.h>

#include "EasySFML.h"
#include "WorldLayer.h"
#include "CButton.h"
#include "CPlayer.h"
#include "CEntity.h"
#include "CItem.h"
#include "ItemManager.h"
#include "Lumber.h"
#include "Globals.h"
#include "CButton.h"
#include "Tool.h"





void GenNewIsland(ItemManager* itemMngr);

void CheckPlayerHitsEdge(CPlayer& player, ItemManager* itemMngr);

void CheckButtonsPressed();

void Drawing(sf::View& view, CPlayer& player);

void CreateEntities(CPlayer& player, ItemManager* itemMngr);

void CreateWindows();

void StartGame();

void GameLoop(ItemManager* itemMngr, CPlayer& player);

struct CButtonManager {
	std::vector<CButton*> buttons;
	sf::Font buttonFont;
	ItemManager* m_itemManager;
	CPlayer* player;

	bool frozenClick = false;
	
	int currentChoice = 0;

	CButtonManager();
}buttonManager;

int main() {
	StartGame();

	

	return 0;
}

void StartGame()
{
	std::string line;
	std::ifstream myfile("Data/seed.txt");
	if (myfile.is_open())
	{
		getline(myfile, line);
		Globals::seed = std::stoi(line);
	}
	else {
		std::ofstream myfile("Data/seed.txt");
		if (myfile.is_open())
		{
			myfile << "1234";
			myfile.close();
		}
		else std::cout << "Failed to Create Seed";
	}

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

	//Create itemmanager 
	ItemManager* itemMngr = new ItemManager();
	buttonManager.m_itemManager = itemMngr;

	//Create Player
	CPlayer player({ 0,0 }, { 20,20 }, sf::Color::Green);
	player.rect.setPosition(WorldLayer::currentWorld->GetFirstSandTilePos());
	buttonManager.player = &player;

	CreateEntities(player, itemMngr);

	float spotlightX = 25, spotlightY = 25;

	GameLoop(itemMngr, player);
}

void GameLoop(ItemManager* itemMngr, CPlayer& player)
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
				return;
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

		CheckButtonsPressed();

		//Keeps Invntory on Top
		HWND hwnd = playerInv->getSystemHandle();
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);


		/*world->resetLightMap();
		sf::Vector2f temp = player.rect.getPosition();
		world->addPointLight((int)(temp.x/20), (int)(temp.y / 20), 9);*/


		Drawing(view, player);


		CheckPlayerHitsEdge(player, itemMngr);
	}
}

//ButtonFunctions
void CheckButtonsPressed()
{
	//Check Mouse lick
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

		//Prevents multiple clicks if holding down
		if (!buttonManager.frozenClick) {

			//Loops through all buttons
			for (CButton* _button : buttonManager.buttons)
			{

				//If click, do func
				if (_button->rect->getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*_button->targetWindow))) {
					if (_button->function != nullptr) _button->function();
				}
			}
		}
		buttonManager.frozenClick = true;
	}
	else {
		buttonManager.frozenClick = false;
	}
}

void GiveItem() {
	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator playerInvIt = Globals::mapOfWindows.find("PlayerInv");
	if (playerInvIt != Globals::mapOfWindows.end()) {
		playerInv = (*playerInvIt).second;
	}
	else {
		std::cout << "ERROR: Cannot Find PlayerInv Window";
	}

	switch (buttonManager.currentChoice)
	{
	case 0:
		buttonManager.m_itemManager->items.push_back(new Lumber(LumberType::Stick, playerInv, sf::Vector2f(10, 10), "Stick"));
		break;

	case 1:
		buttonManager.m_itemManager->items.push_back(new Lumber(LumberType::Log, playerInv, sf::Vector2f(10, 10), "Log"));
		break;

	case 2:
		buttonManager.m_itemManager->items.push_back(new Mineral(MineralType::Stone, playerInv, sf::Vector2f(10, 10), "Stone"));
		break;

	case 3:
		buttonManager.m_itemManager->items.push_back(new Axe(playerInv, sf::Vector2f(10, 10), 100));
		break;

	default:
		break;
	}
	
}

void NextItem() {
	buttonManager.currentChoice++;
	if (buttonManager.currentChoice > 3) buttonManager.currentChoice = 0;

	switch (buttonManager.currentChoice)
	{
	case 0:
		buttonManager.buttons[0]->text->setString("Stick");
		break;

	case 1:
		buttonManager.buttons[0]->text->setString("Log");
		break;

	case 2:
		buttonManager.buttons[0]->text->setString("Stone");
		break;

	case 3:
		buttonManager.buttons[0]->text->setString("Axe");
		break;

	default:
		break;
	}
	
}

void HealPlayer() {
	buttonManager.player->Health = 100;
}

void CreateWindows()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1000, 1000), "2D Game");
	sf::RenderWindow* inventory = new sf::RenderWindow(sf::VideoMode(200, 500), "Inventory");
	sf::RenderWindow* crafting = new sf::RenderWindow(sf::VideoMode(200, 200), "Crafting");
	sf::RenderWindow* debugWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Debug");
	sf::RenderWindow* menuWindow = new sf::RenderWindow(sf::VideoMode(200, 200), "Menu");

	/*window->setVisible(false);
	inventory->setVisible(false);
	crafting->setVisible(false);
	debugWindow->setVisible(false);*/
	menuWindow->setVisible(false);

	inventory->setPosition(sf::Vector2i(window->getPosition().x - inventory->getSize().x, window->getPosition().y));
	crafting->setPosition(sf::Vector2i(inventory->getPosition().x, inventory->getPosition().y + inventory->getSize().y));
	debugWindow->setPosition(sf::Vector2i(window->getPosition().x + window->getSize().x, window->getPosition().y));

	//window.setVerticalSyncEnabled(true);
	window->setFramerateLimit(60);

	//Create Map of Widows and thier ID's
	std::map <std::string, sf::RenderWindow*> windowsMap{
		{ "PlayerInv",inventory },
		{ "WorldInv",window },
		{ "CraftingInv",crafting },
		{ "DebugInv",menuWindow },
		{ "MenuInv",debugWindow }
	};

	//Globals::mapOfWindows = windowsMap;

	for (std::map < std::string, sf::RenderWindow*>::iterator it = windowsMap.begin(); it != windowsMap.end(); it++) {
		Globals::RegisterWindow(it->first, it->second);
	}
	
	buttonManager.buttons.push_back(new CButton(&GiveItem, "Log", buttonManager.buttonFont, 25, sf::Color::White, sf::Text::Style::Bold, 0, 0, sf::Color::Color(0, 150, 0), 5, debugWindow));
	buttonManager.buttons.push_back(new CButton(&NextItem, "Next", buttonManager.buttonFont, 25, sf::Color::White, sf::Text::Style::Bold, 0, 35, sf::Color::Color(0, 150, 0), 5, debugWindow));
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

	//Clear all windows
	std::map < std::string, sf::RenderWindow*>::iterator windIt;
	for (windIt = Globals::mapOfWindows.begin(); windIt != Globals::mapOfWindows.end(); windIt++)
	{
		windIt->second->clear();
	}

	//Render Tiles
	WorldLayer::currentWorld->renderTileMaps();
	//Draw Tiles
	worldInv->draw(*WorldLayer::currentWorld);

	//Add Special Tiles to Draw List
	WorldLayer::currentWorld->DrawSpecial();

	//Update all Objects
	CObjectController::UpdateObjects();

	

	//Center View
	view.setCenter(player.rect.getPosition() + sf::Vector2f(player.rect.getSize().x / 2, player.rect.getSize().y / 2));
	worldInv->setView(view);
	
	//Draw Everything in Todraw List
	for (sf::Drawable* Draw : CWindowUtilities::ToDrawList) //Draw every object on the draw list
	{
		worldInv->draw(*Draw);
	}

	//Draw Buttons
	for (CButton* button : buttonManager.buttons) {
		button->targetWindow->draw(*button->rect);
		button->text->setFont(buttonManager.buttonFont);
		button->targetWindow->draw(*button->text);
	}

	//Dispplay all windows
	for (windIt = Globals::mapOfWindows.begin(); windIt != Globals::mapOfWindows.end(); windIt++)
	{
		windIt->second->display();
	}

	//Clear ToDrawList for next frame
	CWindowUtilities::ToDrawList.clear(); 
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

	if (changedWorld) {
		std::ofstream myfile("Data/seed.txt");
		if (myfile.is_open())
		{
			myfile << Globals::seed;
			myfile.close();
		}
		else std::cout << "Failed to Create Seed";
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

CButtonManager::CButtonManager()
{
	if (!buttonFont.loadFromFile("Resources/Fonts/uni.ttf")) std::cout << "Failed to load uni Font\n";
}
