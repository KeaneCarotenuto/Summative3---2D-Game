#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "EasySFML.h"
#include "CPlayer.h"
#include "CItem.h"
#include "Stackable.h"


class ItemManager:
	private CGameObject
{
public:

	std::vector<sf::RenderWindow*> inventories;

	std::vector<CItem*> items;

	CItem* currentlyDragging = nullptr;
	
	void RemoveItem(CItem* _item);

	virtual void FixedUpdate();

private:
	int currentStep = 0;
};

