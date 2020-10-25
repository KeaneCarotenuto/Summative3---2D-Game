#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "EasySFML.h"
#include "CPlayer.h"
#include "CItem.h"
#include "Stackable.h"
#include "Lumber.h"
#include "Mineral.h"
#include "Consumables.h"

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

	void TryCrafting();

	void RemoveOneItemFromStack(CItem* _itemStack);

	void StartDraggingItem(CItem* _item);

	void UpdateCurrentMouseWindow();

	CItem* SplitOneItem(CItem* _item);

	void StackItem(CItem* _item, CItem* _itemStack);

	void RegisterWindow(std::string _str, sf::RenderWindow* _wind);

	

	operator GameData()
	{
		GameData dat;
		dat.AddGroup(std::string("PlayerInv"));
		dat.AddGroup(std::string("WorldInv"));
		for (CItem* item : items)
		{
			if (!item->bIsEnabled) { continue; }
			//Get name of current window
			std::string currentWindow = "PlayerInv";

			for (GameData::DataGroup& datg : dat.FileData.m_Groups)
			{
				if (datg.GroupID == currentWindow)
				{
					if (dynamic_cast<Resource*>(item))
					{
						datg.m_Groups.push_back(*dynamic_cast<Resource*>(item));
					}
					
					
				}
			}
		}
		
		return dat;
	}

private:
	bool freezeCraftButton = false;
	int currentStep = 0;

	std::map <std::string, sf::RenderWindow*> mapOfWindows;
	

	std::map <std::string, CItem* (*)(sf::RenderWindow* _wind, GameData::DataGroup _datag)> mapOfItems = {
		{"Stick", &Lumber::Stick},
		{"Log", &Lumber::Log},
		{"Stone", &Mineral::Stone},
		{"IronOre", &Mineral::IronOre},
		{"CopperOre", &Mineral::CopperOre},
		{"Meat", &Consumables::Meat},
		{"Berries", &Consumables::Berries},
		{"Water", &Consumables::Water}
	};
};

