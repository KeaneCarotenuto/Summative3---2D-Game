#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "EasySFML.h"
#include "CPlayer.h"
#include "CItem.h"
#include "Stackable.h"
#include "Lumber.h"

class ItemManager:
	private CGameObject, Loadable
{
public:
	ItemManager(std::map < std::string, sf::RenderWindow*> _allWindows);
	~ItemManager();

	std::vector<sf::RenderWindow*> inventories;

	std::vector<CItem*> items;

	CItem* currentlyDragging = nullptr;
	
	void RemoveItem(CItem* _item);

	virtual void FixedUpdate();

	void RegisterWindow(std::string _str, sf::RenderWindow* _wind);

	

private:
	int currentStep = 0;

	std::map <std::string, sf::RenderWindow*> mapOfWindows;

	std::map <std::string, CItem* (*)(sf::RenderWindow* _wind, sf::Vector2f _pos)> mapOfItems = {
		{"Stick", &Lumber::Stick},
		{"Log", &Lumber::Log}
	};
};

