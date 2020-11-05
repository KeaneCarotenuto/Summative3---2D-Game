////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	CItem
//					Base class for items
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

#include <iostream>
#include "GameData.h"
#include "EasySFML.h"
#include "Globals.h"


class CItem :
	public CGameObject, public Loadable
{
public:

	std::string itemName;

	sf::Texture texture;
	sf::Sprite sprite;

	//static ItemManager* itemManager;
	sf::RenderWindow* currentInv;
	std::string currentInvName;

	sf::Vector2f initialPos;
	sf::RenderWindow* initialWindow;

	bool operator==(const CItem& _item);

	virtual operator GameData::DataGroup() = 0;
	

	virtual void FixedUpdate();
	virtual void Update(float _fDeltaTime);

	
protected:
	CItem(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, std::string _filepath);

	virtual void Draw();
	virtual void Draw(sf::Vector2f);
};

