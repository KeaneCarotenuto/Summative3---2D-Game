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
#include "WorldLayer.h"
#include "CEntity.h"
#include "Globals.h"


class ItemManager:
	private CGameObject, Loadable
{
public:
	ItemManager();
	~ItemManager();

	std::vector<CItem*> items;

	std::vector<CEntity*> entities;

	std::vector<SpecialTile*> toDeleteSpecial;
	std::vector<CEntity*> toDeleteEnt;

	sf::RenderWindow* currentMouseWindow = nullptr;

	WorldLayer* world;

	CItem* currentlyDragging = nullptr;
	
	void RemoveItem(CItem* _item);

	virtual void FixedUpdate();

	void CheckEntities(sf::RenderWindow* worldInv);

	void CheckSpecialTiles(sf::RenderWindow* worldInv);

	bool AddToToDeleteSpecial(SpecialTile* _tile);

	bool AddToToDeleteEnt(CEntity* _ent);

	void LateDelete();

	void TryCrafting();

	void SpawnMapItems();

	void RemoveOneItemFromStack(CItem* _itemStack);

	void StartDraggingItem(CItem* _item);

	void UpdateCurrentMouseWindow();

	CItem* SplitOneItem(CItem* _item);

	void StackItem(CItem* _item, CItem* _itemStack);

	

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

