////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	CEntity
//					An entity that moves in the game world
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
#include "EasySFML.h"
#include "CPlayer.h"

class ItemManager;

enum class EntityType
{
	Bird,
	Fish
};

class CEntity :
	public CGameObject
{
public:
	sf::RectangleShape rect;
	float moveSpeed;
	int currentStep = 0;
	EntityType type;

	sf::Vector2f velocity;

	CPlayer* player;

	ItemManager* itemManager;

	void Movement();

	virtual void Update(float _fDeltaTime);
	virtual void FixedUpdate();

	CEntity(EntityType _type ,sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col);

};

