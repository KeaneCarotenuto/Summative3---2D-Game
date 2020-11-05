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
	float moveSpeed = 5;

	int Health;
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

private:
	int currentStep = 0;
};

