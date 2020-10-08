#include "ItemManager.h"

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
		for (sf::RenderWindow* _wind : inventories)
		{
			if (_wind->getPosition().x <= sf::Mouse::getPosition().x && _wind->getPosition().x + _wind->getSize().x >= sf::Mouse::getPosition().x
				&& _wind->getPosition().y <= sf::Mouse::getPosition().y && _wind->getPosition().y + _wind->getSize().y >= sf::Mouse::getPosition().y) {
				currentMouseWindow = _wind;
				break;
			}
			else {
				currentMouseWindow = nullptr;
			}
		}

		//Start trying to drag an object if not current doing so
		if (currentlyDragging == nullptr && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			for (CItem* _item : items) {
				if (!_item->bIsEnabled) continue;

				sf::Sprite sprite = _item->sprite;

				//If mouse is over item, being dragging
				if (sprite.getGlobalBounds().contains(_item->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*_item->currentInv)))) {
					currentlyDragging = _item;
					_item->initialPos = sprite.getPosition();
					_item->initialWindow = _item->currentInv;
					break;
				}
			}
		}

		//Left Click To Drag
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			//Update items current window, if one exists
			if (currentMouseWindow != nullptr) currentlyDragging->currentInv = currentMouseWindow;

			//Update items current position to mouse
			currentlyDragging->sprite.setPosition(currentlyDragging->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*currentlyDragging->currentInv)).x - currentlyDragging->sprite.getGlobalBounds().width / 2,
				currentlyDragging->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*currentlyDragging->currentInv)).y - currentlyDragging->sprite.getGlobalBounds().height / 2);
		}

		//Right Click to Split One off
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && currentStep % 5 == 0 ) {
			for (CItem* _item : items) {
				if (!_item->bIsEnabled) continue;

				sf::Sprite sprite = _item->sprite;

				if (!_item->disabledStack.empty()) {
					//If mouse is over item, being dragging
					if (sprite.getGlobalBounds().contains(_item->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*_item->currentInv)))) {
						
						_item->disabledStack.top()->bIsEnabled = true;
						_item->disabledStack.top()->sprite.setPosition(sprite.getPosition().x + 10, sprite.getPosition().y + 10);

						_item->disabledStack.pop();

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
		else if (currentlyDragging != nullptr && currentMouseWindow != nullptr) {
			for (CItem* _item : items)
			{
				if (!_item->bIsEnabled) continue;

				sf::Sprite sprite = _item->sprite;
				if (sprite.getGlobalBounds().contains(_item->currentInv->mapPixelToCoords(sf::Mouse::getPosition(*_item->currentInv)))) {
					if (currentlyDragging->type == _item->type && currentlyDragging != _item) {
						
						_item->disabledStack.push(currentlyDragging);
						currentlyDragging->bIsEnabled = false;

						while (!currentlyDragging->disabledStack.empty()) {
							_item->disabledStack.push(currentlyDragging->disabledStack.top());
							currentlyDragging->disabledStack.pop();
						}

						break;
					}
				}
			}
		}

		//Deref
		currentlyDragging = nullptr;
	}

	currentStep++;
}
