#pragma warning(disable : 6011)
#include "ItemManager.h"



ItemManager::ItemManager():
	Loadable("Items/", "Manager")
{
	//Registers All Windows handed to it under map with name and pointer
	for (std::map < std::string, sf::RenderWindow*>::iterator it = Globals::mapOfWindows.begin(); it != Globals::mapOfWindows.end(); it++) {
		RegisterWindow(it->first, it->second);
	}

	//For every inventory Group (e.g, PlayerInv or WorldInv), get the data in the group
	for (GameData::DataGroup _group : Data->FileData.m_Groups) {
		//For all of the data in that group (e.g. {"Stick","10"}), get that data
		for (GameData::DataGroup& _childgroup : _group.m_Groups) {

			//Find the window that the group is a part of, from the map of windows, using the Group ID
			std::map < std::string, sf::RenderWindow*>::iterator windIt = Globals::mapOfWindows.find(_group.GroupID);
			if (windIt != Globals::mapOfWindows.end()) {

				

					//Find the item constructor function from the map of items, using the data id from the file
					std::map <std::string, CItem* (*)(sf::RenderWindow* _wind, GameData::DataGroup _datag)>::iterator itemIt = mapOfItems.find(_childgroup.GroupID);
					if (itemIt != mapOfItems.end()) {

						//Create the actual item and push it to the list of items

						items.push_back(itemIt->second(windIt->second, _childgroup));
						

					}
				

			}
			
		}
	}

	
	
}

ItemManager::~ItemManager()
{
	GameData SaveData(*this);
	SaveData.Save(FilePath, "Manager");
}

void ItemManager::RemoveItem(CItem* _item)
{
	std::vector<CItem*>::iterator pos = std::find(items.begin(), items.end(), _item);
	if (pos != items.end()) {
		items.erase(pos);
		delete _item;
	}
}


void ItemManager::FixedUpdate()
{

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{

		//Update Which window the mouse is currently over
		UpdateCurrentMouseWindow();

		//Start trying to drag an object if not current doing so
		if (currentlyDragging == nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			for (CItem* _item : items) {
				if (!_item->bIsEnabled) continue;

				sf::Sprite sprite = _item->sprite;

				//If mouse is over item, being dragging
				if (sprite.getGlobalBounds().contains(_item->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*_item->currentInv)))) {
					StartDraggingItem(_item);
					break;
				}
			}
		}

		//Left Click To Drag
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//Update items current window, if one exists
			if (currentMouseWindow != nullptr) currentlyDragging->currentInv = currentMouseWindow;

			//Update items current position to mouse
			currentlyDragging->sprite.setPosition(	currentlyDragging->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*currentlyDragging->currentInv)).x - currentlyDragging->sprite.getGlobalBounds().width / 2,
													currentlyDragging->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*currentlyDragging->currentInv)).y - currentlyDragging->sprite.getGlobalBounds().height / 2);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && currentStep % 5 == 0) {
				sf::RenderWindow* worldInv = nullptr;
				std::map < std::string, sf::RenderWindow*>::iterator invWndIt = Globals::mapOfWindows.find("WorldInv");
				if (invWndIt != Globals::mapOfWindows.end()) {
					worldInv = (*invWndIt).second;
				}

				CheckSpecialTiles(worldInv);

				CheckEntities(worldInv);
			}
		}

		//Right Click to Split One off
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && currentStep % 5 == 0 ) {
			for (CItem* _item : items) {
				if (!_item->bIsEnabled || !dynamic_cast<ItemAttributes::Stackable*>(_item)) continue;

				sf::Sprite sprite = _item->sprite;

				if (!dynamic_cast<ItemAttributes::Stackable*>(_item)->disabledStack.empty()) {
					//If mouse is over item, being dragging
					if (sprite.getGlobalBounds().contains(_item->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*_item->currentInv)))) {
						
						SplitOneItem(_item);

						break;
					}
				}
				
			}
		}
	}
	else {

		//If non suitable window found, place item back in original pos and window
		if (currentMouseWindow == nullptr && currentlyDragging != nullptr) {
			currentlyDragging->sprite.setPosition(currentlyDragging->initialPos);
			currentlyDragging->currentInv = currentlyDragging->initialWindow;
		}
		//Stack Items
		
		if (currentlyDragging != nullptr && currentMouseWindow != nullptr) {
			for (CItem* _item : items)
			{
				if (!_item->bIsEnabled || !dynamic_cast<ItemAttributes::Stackable*>(_item)) continue;

				sf::Sprite sprite = _item->sprite;
				if (sprite.getGlobalBounds().contains(_item->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*_item->currentInv)))) {

					if (currentlyDragging->itemName == _item->itemName && currentlyDragging != _item) {
						
						StackItem(_item, currentlyDragging);
						break;
					}
				}
			}
		}

		//Deref
		currentlyDragging = nullptr;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		if (!freezeCraftButton) {
			freezeCraftButton = true;
			TryCrafting();
		}
	}
	else {
		freezeCraftButton = false;
	}


	currentStep++;
}

void ItemManager::CheckEntities(sf::RenderWindow* worldInv)
{
	for (CEntity* _ent : entities) {
		if (_ent == nullptr) continue;

		sf::RectangleShape rect = _ent->rect;
		if (rect.getGlobalBounds().contains(worldInv->mapPixelToCoords(sf::Mouse::getPosition(*worldInv)))) {


			if (currentlyDragging->itemName == "Stick") {

				if (AddToToDeleteEnt(_ent)) {
					items.push_back(new Consumables(ConsumableType::Meat, worldInv, _ent->rect.getPosition(), "Meat"));

					_ent = nullptr;
				}
			}
		}
	}
}

void ItemManager::CheckSpecialTiles(sf::RenderWindow* worldInv)
{
	for (int y = 0; y < 500; y++)
	{
		for (int x = 0; x < 500; x++)
		{
			if (world->SpecialTilemap[x][y] == nullptr) continue;

			sf::Sprite sprite = world->SpecialTilemap[x][y]->Sprite;
			if (sprite.getGlobalBounds().contains(worldInv->mapPixelToCoords(sf::Mouse::getPosition(*worldInv)))) {


				if (currentlyDragging->itemName == "Stone") {

					if (AddToToDeleteSpecial(world->SpecialTilemap[x][y])) {
						items.push_back(new Lumber(LumberType::Log, worldInv, sprite.getPosition(), "Log"));
						items.push_back(new Lumber(LumberType::Stick, worldInv, sprite.getPosition(), "Stick"));

						world->SpecialTilemap[x][y] = nullptr;
					}
					

				}
			}
		}
	}
}

bool ItemManager::AddToToDeleteSpecial(SpecialTile* _tile)
{
	std::vector<SpecialTile*>::iterator pos = std::find(toDeleteSpecial.begin(), toDeleteSpecial.end(), _tile);
	if (pos != toDeleteSpecial.end()) {
		return false;
	}
	else {
		toDeleteSpecial.push_back(_tile);
		return true;
	}
}

bool ItemManager::AddToToDeleteEnt(CEntity* _ent)
{
	std::vector<CEntity*>::iterator pos = std::find(toDeleteEnt.begin(), toDeleteEnt.end(), _ent);
	if (pos != toDeleteEnt.end()) {
		return false;
	}
	else {
		toDeleteEnt.push_back(_ent);

		pos = std::find(entities.begin(), entities.end(), _ent);
		if (pos != entities.end()) {
			entities.erase(pos);
		}

		return true;
	}

	

	
}

void ItemManager::LateDelete()
{
	for (SpecialTile* _spTile : toDeleteSpecial) {

		delete _spTile;

		_spTile = nullptr;
	}

	for (CEntity* _ent : toDeleteEnt) {
		delete _ent;

		_ent = nullptr;

	}

	toDeleteSpecial.clear();
	toDeleteEnt.clear();
}

void ItemManager::TryCrafting()
{
	sf::RenderWindow* craftingInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator craftWndIt = Globals::mapOfWindows.find("CraftingInv");
	if (craftWndIt != Globals::mapOfWindows.end()) {
		craftingInv = (*craftWndIt).second;
	}
	else {
		return;
	}

	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator invWndIt = Globals::mapOfWindows.find("PlayerInv");
	if (invWndIt != Globals::mapOfWindows.end()) {
		playerInv = (*invWndIt).second;
	}
	else {
		return;
	}


	std::vector<CItem*> inCrafting;
	for (CItem* _item : items)
	{
		if (_item->currentInv == craftingInv) {
			inCrafting.push_back(_item);
		}
	}

	std::vector<CItem*> stickStack;
	std::vector<CItem*> logStack;
	std::vector<CItem*> meatStack;
	std::vector<CItem*> berriesStack;

	for (CItem* _item : inCrafting)
	{
		if (dynamic_cast<Lumber*>(_item)) {
			if (dynamic_cast<Lumber*>(_item)->type == LumberType::Stick) {
				stickStack.push_back(_item);
			}
			if (dynamic_cast<Lumber*>(_item)->type == LumberType::Log) {
				logStack.push_back(_item);
			}
		}
		if (dynamic_cast<Consumables*>(_item)) {
			if (dynamic_cast<Consumables*>(_item)->type == ConsumableType::Meat) {
				meatStack.push_back(_item);
			}
			if (dynamic_cast<Consumables*>(_item)->type == ConsumableType::Berries) {
				berriesStack.push_back(_item);
			}
		}
	}

	if (!stickStack.empty() && !berriesStack.empty()) {
		RemoveOneItemFromStack(stickStack[0]);
		
		RemoveOneItemFromStack(berriesStack[0]);

		CItem* craftedItem = new Consumables(ConsumableType::Water, craftingInv, { 10,10 }, "Water");
		items.push_back(craftedItem);
		craftedItem->sprite.setColor(sf::Color(255, 255, 0));
	}
}

void ItemManager::SpawnMapItems()
{
	sf::RenderWindow* worldInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator invWndIt = Globals::mapOfWindows.find("WorldInv");
	if (invWndIt != Globals::mapOfWindows.end()) {
		worldInv = (*invWndIt).second;
	}
	else {
		std::cout << "Failed to Find World Inv\n";
		return;
	}

	for (int x = 0; x < WorldLayer::width; x++) {
		for (int y = 0; y < WorldLayer::height; y++) {
			if (world->TerrainTilemap[x][y]->Type == TerrainType::ROCK && rand() % 500 == 0) {
				items.push_back(new Mineral(MineralType::Stone, worldInv, sf::Vector2f(x*20, y*20), "Stone"));
			}

			if (world->TerrainTilemap[x][y]->Type == TerrainType::GRASS && rand() % 500 == 0) {
				items.push_back(new Lumber(LumberType::Stick, worldInv, sf::Vector2f(x * 20, y * 20), "Stick"));
			}
		}
	}
}

void ItemManager::RemoveOneItemFromStack(CItem* _itemStack)
{
	if (dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.empty()) {
		RemoveItem(_itemStack);
	}
	else {
		CItem* splitItem = SplitOneItem(_itemStack);
		RemoveItem(splitItem);
	}
}

void ItemManager::StartDraggingItem(CItem* _item)
{
	_item->sprite.setColor(sf::Color(255, 255, 255));
	_item->initialPos = _item->sprite.getPosition();
	_item->initialWindow = _item->currentInv;
	currentlyDragging = _item;
}

void ItemManager::UpdateCurrentMouseWindow()
{
	for (sf::RenderWindow* _wind : inventories)
	{
		if (_wind->getPosition().x <= sf::Mouse::getPosition().x && (signed)_wind->getPosition().x + (signed)_wind->getSize().x >= sf::Mouse::getPosition().x
			&& _wind->getPosition().y <= sf::Mouse::getPosition().y && (signed)_wind->getPosition().y + (signed)_wind->getSize().y >= sf::Mouse::getPosition().y) {
			currentMouseWindow = _wind;
			break;
		}
		else {
			currentMouseWindow = nullptr;
		}
	}
}

CItem* ItemManager::SplitOneItem(CItem* _itemStack)
{
	CItem* toReturn = dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top();

	dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top()->bIsEnabled = true;
	dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top()->sprite.setPosition(_itemStack->sprite.getPosition().x + 10, _itemStack->sprite.getPosition().y + 10);
	dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top()->initialPos = { _itemStack->sprite.getPosition().x + 10, _itemStack->sprite.getPosition().y + 10 };
	dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top()->initialWindow = _itemStack->currentInv;
	dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top()->currentInv = _itemStack->currentInv;
	
	items.push_back(dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top());


	dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.pop();

	return toReturn;
}

void ItemManager::StackItem(CItem* _item, CItem* _itemStack)
{
	dynamic_cast<ItemAttributes::Stackable*>(_item)->disabledStack.push(_itemStack);
	_itemStack->bIsEnabled = false;

	while (!dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.empty()) {
		dynamic_cast<ItemAttributes::Stackable*>(_item)->disabledStack.push(dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top());
		dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.pop();
	}
	std::vector<CItem*>::iterator pos = std::find(items.begin(), items.end(), _itemStack);
	if (pos != items.end()) {
		items.erase(pos);
	}
}

void ItemManager::RegisterWindow(std::string _str, sf::RenderWindow* _wind)
{
	inventories.push_back(_wind);
}
