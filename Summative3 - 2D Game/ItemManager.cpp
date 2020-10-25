#include "ItemManager.h"
#pragma warning(disable : 6011)



ItemManager::ItemManager(std::map < std::string, sf::RenderWindow*> _allWindows):
	Loadable("Items/", "Manager")
{
	//Registers All Windows handed to it under map with name and pointer
	for (std::map < std::string, sf::RenderWindow*>::iterator it = _allWindows.begin(); it != _allWindows.end(); it++) {
		RegisterWindow(it->first, it->second);
	}

	//For every inventory Group (e.g, PlayerInv or WorldInv), get the data in the group
	for (GameData::DataGroup _group : Data->FileData.m_Groups) {
		//For all of the data in that group (e.g. {"Stick","10"}), get that data
		for (GameData::DataGroup& _childgroup : _group.m_Groups) {

			//Find the window that the group is a part of, from the map of windows, using the Group ID
			std::map < std::string, sf::RenderWindow*>::iterator windIt = mapOfWindows.find(_group.GroupID);
			if (windIt != mapOfWindows.end()) {

				

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

sf::RenderWindow* currentMouseWindow = nullptr;
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

void ItemManager::TryCrafting()
{
	sf::RenderWindow* craftingInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator craftWndIt = mapOfWindows.find("CraftingInv");
	if (craftWndIt != mapOfWindows.end()) {
		craftingInv = (*craftWndIt).second;
	}
	else {
		return;
	}

	sf::RenderWindow* playerInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator invWndIt = mapOfWindows.find("PlayerInv");
	if (invWndIt != mapOfWindows.end()) {
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

	mapOfWindows[_str] = _wind;
	
}
