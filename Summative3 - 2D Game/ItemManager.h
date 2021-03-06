////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	ItemManager
//					Manages items, crafting, entities, and some other stuff
//--------------------------------------------------------//
//					BSE20021
//    Author    :	Nerys Thamm
//					Keane Carotenuto
//--------------------------------------------------------//
//    E-mails   :	NerysThamm@gmail.com
//					Keane Carotenuto@gmail.com
//========================================================//
////////////////////////////////////////////////////////////

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
#include "CEntity.h"
#include "WorldLayer.h"
#include "Globals.h"
#include "Tool.h"


class ItemManager:
	private CGameObject, Loadable
{
public:
	ItemManager(CPlayer*& _player);
	~ItemManager();

	CPlayer*& Player;

	std::vector<CItem*> items;

	std::vector<CEntity*> entities;

	std::vector<SpecialTile*> toDeleteSpecial;
	std::vector<CEntity*> toDeleteEnt;
	std::vector<CItem*> toDeleteItem;

	sf::RenderWindow* currentMouseWindow = nullptr;

	CItem* currentlyDragging = nullptr;
	
	void RemoveItem(CItem* _item);

	virtual void FixedUpdate();
	
	virtual void Update(float _fDeltaTime);

	void CheckEntities(sf::RenderWindow* worldInv);

	void CheckSpecialTiles(sf::RenderWindow* worldInv);

	bool DamageSpecialTile(int x, int y, int _damage);

	bool AddToToDeleteEnt(CEntity* _ent);

	bool AddToToDeleteItem(CItem* _item);

	void LateDelete();

	void TryCrafting();

	void TrySpawnItem(CItem* _item, bool tryStack = true);

	void SpawnMapItems();

	void RemoveOneItemFromStack(CItem* _itemStack);

	void StartDraggingItem(CItem* _item);

	void UpdateCurrentMouseWindow();

	CItem* SplitOneItem(CItem* _item);

	void StackItem(CItem* _item, CItem* _itemStack);

	

	operator GameData()
	{
		sf::RenderWindow* playerInv = nullptr;
		std::map < std::string, sf::RenderWindow*>::iterator invWndIt = Globals::mapOfWindows.find("PlayerInv");
		if (invWndIt != Globals::mapOfWindows.end()) {
			playerInv = (*invWndIt).second;
		}

		GameData dat;
		dat.AddGroup(std::string("PlayerInv"));
		dat.AddGroup(std::string("WorldInv"));
		for (CItem* item : items)
		{
			if (!item->bIsEnabled || item->currentInv != playerInv) { continue; }
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
					if (dynamic_cast<Tool*>(item))
					{
						datg.m_Groups.push_back(*dynamic_cast<Tool*>(item));
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
		{"Water", &Consumables::Water},
		{"Axe", &Tool::Axe},
		{"Pickaxe", &Tool::Pickaxe},
		{"Sword", &Tool::Sword}
		
	};
};

