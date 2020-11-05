////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	CPlayer
//					The player Class
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
#include "GameData.h"
#include "WorldLayer.h"

class CPlayer:
	public CGameObject, public Loadable
{
public:
	sf::RectangleShape rect;
	sf::Text HealthText;
	sf::Text HungerText;
	sf::Font Font;
	float moveSpeed = 5;

	float Health;
	float Hunger;
	
	operator GameData()
	{
		GameData dat;

		dat.FileData.GroupID = "PlayerData";
		dat.AddVariable(Variable(Health));
		dat.AddVariable(Variable(Hunger));
		return dat;
	}

	CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col);
	~CPlayer();

	void Movement();
	void ScreenWrap();

	virtual void Update(float _fDeltaTime);
	virtual void FixedUpdate();

	int currentStep = 0;
	int deathStep = 0;
	bool dead = false;
};

