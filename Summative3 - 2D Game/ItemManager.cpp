#include "ItemManager.h" 
#pragma warning(disable : 6011) 



ItemManager::ItemManager(CPlayer*& _player) :
	Player(_player) ,Loadable("Items/", "Manager")
{

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

						TrySpawnItem(itemIt->second(windIt->second, _childgroup));
						

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
		AddToToDeleteItem(_item);
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

					CheckSpecialTiles(worldInv);

					CheckEntities(worldInv);

					if (currentlyDragging != nullptr) {
						if (currentlyDragging->itemName == "Meat") {
							Player->Health += 30;
							Player->Hunger += 60;
							AddToToDeleteItem(currentlyDragging);
						}
					}
				}

				
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
			currentlyDragging->initialPos = currentlyDragging->sprite.getPosition();
			currentlyDragging->initialWindow = currentlyDragging->currentInv;

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

void ItemManager::Update(float _fDeltaTime)
{
}

void ItemManager::CheckEntities(sf::RenderWindow* worldInv)
{
	for (CEntity* _ent : entities) {
		if (_ent == nullptr) continue;

		sf::RectangleShape rect = _ent->rect;
		if (rect.getGlobalBounds().contains(worldInv->mapPixelToCoords(sf::Mouse::getPosition(*worldInv)))) {


			if (currentlyDragging->itemName == "Stick") {

				if (AddToToDeleteEnt(_ent)) {
					TrySpawnItem(new Consumables(ConsumableType::Meat, worldInv, _ent->rect.getPosition(), "Meat"));
				}
			}
		}
	}

	
}

void ItemManager::CheckSpecialTiles(sf::RenderWindow* worldInv)
{
	for (int y = 0; y < WorldLayer::height; y++)
	{
		for (int x = 0; x < WorldLayer::width; x++)
		{
			if (WorldLayer::currentWorld->SpecialTilemap[x][y] == nullptr) continue;

			sf::Sprite sprite = WorldLayer::currentWorld->SpecialTilemap[x][y]->Sprite;
			if (sprite.getGlobalBounds().contains(worldInv->mapPixelToCoords(sf::Mouse::getPosition(*worldInv)))) {


				if (currentlyDragging->itemName == "Axe" && WorldLayer::currentWorld->SpecialTilemap[x][y]->type == SpecialType::Tree) {
					if (DamageSpecialTile(x, y, 20)) {
						dynamic_cast<Tool*>(currentlyDragging)->Durability -= 5;

						TrySpawnItem(new Lumber(LumberType::Log, worldInv, sprite.getPosition(), "Log"));

						if (rand() % 2 == 0) {
							TrySpawnItem(new Lumber(LumberType::Stick, worldInv, sprite.getPosition(), "Stick"));
						}
						
					}
				}
				else if (currentlyDragging->itemName == "Pickaxe" && WorldLayer::currentWorld->SpecialTilemap[x][y]->type == SpecialType::Boulder) {
					if (DamageSpecialTile(x, y, 20)) {
						dynamic_cast<Tool*>(currentlyDragging)->Durability -= 7;

						TrySpawnItem(new Mineral(MineralType::Stone, worldInv, sprite.getPosition(), "Stone"));
					}
				}
			}
		}
	}
}

bool ItemManager::DamageSpecialTile(int x, int y, int _damage)
{
	WorldLayer::currentWorld->SpecialTilemap[x][y]->health -= _damage;

	if (WorldLayer::currentWorld->SpecialTilemap[x][y]->health <= 0) {
		std::vector<SpecialTile*>::iterator pos = std::find(toDeleteSpecial.begin(), toDeleteSpecial.end(), WorldLayer::currentWorld->SpecialTilemap[x][y]);
		if (pos != toDeleteSpecial.end()) {
			return false;
		}
		else {
			toDeleteSpecial.push_back(WorldLayer::currentWorld->SpecialTilemap[x][y]);
			WorldLayer::currentWorld->SpecialTilemap[x][y] = nullptr;
			return true;
		}
	}
	else {
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
		

		pos = std::find(entities.begin(), entities.end(), _ent);
		if (pos != entities.end()) {
			toDeleteEnt.push_back(_ent);
			entities.erase(pos);
			return true;
		}
		else return false;

		
	}
}

bool ItemManager::AddToToDeleteItem(CItem* _item)
{
	std::vector<CItem*>::iterator pos = std::find(toDeleteItem.begin(), toDeleteItem.end(), _item);
	if (pos != toDeleteItem.end()) {
		return false;
	}
	else {


		pos = std::find(items.begin(), items.end(), _item);
		if (pos != items.end()) {
			toDeleteItem.push_back(_item);
			items.erase(pos);
			return true;
		}
		else return false;


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

	for (CItem* _item : toDeleteItem) {
		delete _item;

		_item = nullptr;
	}

	toDeleteSpecial.clear();
	toDeleteEnt.clear();
	toDeleteItem.clear();

	for (CItem* _item : items) {
		if (_item != nullptr) {
			if (dynamic_cast<Tool*>(_item)) {
				if (dynamic_cast<Tool*>(_item)->Durability <= 0) {

					std::vector<CItem*>::iterator pos = std::find(items.begin(), items.end(), _item);
					if (pos != items.end()) {
						delete _item;
						_item = nullptr;
						items.erase(pos);
					}
				}
			}
		}
	}
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

	std::vector<CItem*> stickStacks;
	std::vector<CItem*> logStacks;
	std::vector<CItem*> stoneStacks;
	std::vector<CItem*> ironOreStacks;
	std::vector<CItem*> copperOreStacks;
	std::vector<CItem*> meatStacks;
	std::vector<CItem*> berriesStacks;
	std::vector<CItem*> axeStacks;
	std::vector<CItem*> pickaxeStacks;

	for (CItem* _item : inCrafting)
	{
		if (dynamic_cast<Lumber*>(_item)) {
			if (dynamic_cast<Lumber*>(_item)->type == LumberType::Stick) {
				stickStacks.push_back(_item);
			}
			if (dynamic_cast<Lumber*>(_item)->type == LumberType::Log) {
				logStacks.push_back(_item);
			}
		}
		if (dynamic_cast<Consumables*>(_item)) {
			if (dynamic_cast<Consumables*>(_item)->type == ConsumableType::Meat) {
				meatStacks.push_back(_item);
			}
			if (dynamic_cast<Consumables*>(_item)->type == ConsumableType::Berries) {
				berriesStacks.push_back(_item);
			}
		}
		if (dynamic_cast<Mineral*>(_item)) {
			if (dynamic_cast<Mineral*>(_item)->type == MineralType::Stone) {
				stoneStacks.push_back(_item);
			}
			if (dynamic_cast<Mineral*>(_item)->type == MineralType::IronOre) {
				ironOreStacks.push_back(_item);
			}
			if (dynamic_cast<Mineral*>(_item)->type == MineralType::CopperOre) {
				copperOreStacks.push_back(_item);
			}
		}
		if (dynamic_cast<Tool*>(_item)) {
			if (dynamic_cast<Tool*>(_item)->type == ToolType::Axe) {
				axeStacks.push_back(_item);
			}
			if (dynamic_cast<Tool*>(_item)->type == ToolType::Pickaxe) {
				pickaxeStacks.push_back(_item);
			}
		}

	}

	if (stickStacks.size()		== 1 && 
		logStacks.size()		== 0 &&
		axeStacks.size()		== 0 &&
		pickaxeStacks.size()	== 0 &&
		stoneStacks.size()		== 0 &&
		ironOreStacks.size()	== 0 &&
		copperOreStacks.size()	== 0 &&
		meatStacks.size()		== 0 &&
		berriesStacks.size()	== 1 ) 
	{
		//Specific To Crafting Recipie
		RemoveOneItemFromStack(stickStacks[0]);
		RemoveOneItemFromStack(berriesStacks[0]);

		CItem* craftedItem = new Consumables(ConsumableType::Water, playerInv, { 10,10 }, "Water");
		
		//Same for all Recipies
		craftedItem->sprite.setColor(sf::Color(255, 255, 0));
		TrySpawnItem(craftedItem);
	}

	if (stickStacks.size() == 0 &&
		logStacks.size() == 1 &&
		stoneStacks.size() == 0 &&
		axeStacks.size() == 1 &&
		pickaxeStacks.size() == 0 &&
		ironOreStacks.size() == 0 &&
		copperOreStacks.size() == 0 &&
		meatStacks.size() == 0 &&
		berriesStacks.size() == 0)
	{
		//Specific To Crafting Recipie
		RemoveOneItemFromStack(logStacks[0]);

		CItem* craftedItem = new Lumber(LumberType::Stick, playerInv, { 10,10 }, "Stick");

		//Same for all Recipies
		craftedItem->sprite.setColor(sf::Color(255, 255, 0));
		TrySpawnItem(craftedItem);
	}

	if (stickStacks.size() == 3 &&
		logStacks.size() == 0 &&
		stoneStacks.size() == 2 &&
		axeStacks.size() == 0 &&
		pickaxeStacks.size() == 0 &&
		ironOreStacks.size() == 0 &&
		copperOreStacks.size() == 0 &&
		meatStacks.size() == 0 &&
		berriesStacks.size() == 0)
	{
		//Specific To Crafting Recipie
		RemoveOneItemFromStack(stickStacks[0]);
		RemoveOneItemFromStack(stickStacks[1]);
		RemoveOneItemFromStack(stickStacks[2]);

		RemoveOneItemFromStack(stoneStacks[0]);
		RemoveOneItemFromStack(stoneStacks[1]);

		CItem* craftedItem = Tool::Axe(playerInv, { 10,10 });

		//Same for all Recipies
		craftedItem->sprite.setColor(sf::Color(255, 255, 0));
		TrySpawnItem(craftedItem);
	}

	if (stickStacks.size() == 2 &&
		logStacks.size() == 0 &&
		stoneStacks.size() == 3 &&
		axeStacks.size() == 0 &&
		pickaxeStacks.size() == 0 &&
		ironOreStacks.size() == 0 &&
		copperOreStacks.size() == 0 &&
		meatStacks.size() == 0 &&
		berriesStacks.size() == 0)
	{
		//Specific To Crafting Recipie
		RemoveOneItemFromStack(stickStacks[0]);
		RemoveOneItemFromStack(stickStacks[1]);

		RemoveOneItemFromStack(stoneStacks[0]);
		RemoveOneItemFromStack(stoneStacks[1]);
		RemoveOneItemFromStack(stoneStacks[2]);

		CItem* craftedItem = Tool::Pickaxe(playerInv, { 10,10 });

		//Same for all Recipies
		craftedItem->sprite.setColor(sf::Color(255, 255, 0));
		TrySpawnItem(craftedItem);
	}
}

void ItemManager::TrySpawnItem(CItem * _item, bool tryStack)
{
	if (items.size() < 1000) {
		items.push_back(_item);

		if (tryStack) {
			if (dynamic_cast<ItemAttributes::Stackable*>(_item)) {
				for (CItem* tempItem : items) {
					if (tempItem == _item) continue;

					if (tempItem->sprite.getGlobalBounds().intersects(_item->sprite.getGlobalBounds()) && tempItem->itemName == _item->itemName) {
						StackItem(tempItem, _item);
						break;
					}
				}
			}
		}
		
	}
	else {
		if (items.size() > 1) {
			std::cout << "E\n";
		}

		std::cout << "Too many Items\n";
		delete _item;
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
			if (WorldLayer::currentWorld->TerrainTilemap[x][y]->Type == TerrainType::ROCK && rand() % 500 == 0) {
				TrySpawnItem(new Mineral(MineralType::Stone, worldInv, sf::Vector2f(x*20, y*20), "Stone"));
			}

			if (WorldLayer::currentWorld->TerrainTilemap[x][y]->Type == TerrainType::GRASS && rand() % 500 == 0) {
				TrySpawnItem(new Lumber(LumberType::Stick, worldInv, sf::Vector2f(x * 20, y * 20), "Stick"));
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
	for (sf::RenderWindow* _wind : Globals::inventories)
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
	
	TrySpawnItem(dynamic_cast<ItemAttributes::Stackable*>(_itemStack)->disabledStack.top(), false);


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
