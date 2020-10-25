#pragma once
#include <SFML/Graphics.hpp>
#include "EasySFML.h"
#include "GameData.h"
#include "WorldLayer.h"




class CPlayer:
	private CGameObject, public Loadable
{
public:
	sf::RectangleShape rect;
	float moveSpeed = 20;
	WorldLayer* currentWorld;

	int Health;
	float Hunger;
	float Thirst;
	bool IsBleeding;

	int Str;
	int Agi;
	int Wis;
	int Def;

	std::vector<std::string> Effects;
	std::vector<std::vector<int>> TestVector;
	
	operator GameData()
	{
		GameData dat;

		dat.FileData.GroupID = "PlayerData";
		dat.AddVariable(Variable(Health));
		dat.AddVariable(Variable(Hunger));
		dat.AddVariable(Variable(Thirst));
		dat.AddVariable(Variable(IsBleeding));

		dat.AddVariable(Variable(Str));
		dat.AddVariable(Variable(Agi));
		dat.AddVariable(Variable(Wis));
		dat.AddVariable(Variable(Def));

		dat.AddGroup(Group(Effects));
		dat.AddGroup(Group(TestVector));

		return dat;
	}

	CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col, WorldLayer* _world);
	~CPlayer();

	void Movement();
	void ScreenWrap();

	virtual void FixedUpdate();

private:
	int currentStep = 0;
};

