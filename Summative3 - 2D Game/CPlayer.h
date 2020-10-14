#pragma once
#include <SFML/Graphics.hpp>
#include "EasySFML.h"
#include "GameData.h"




class CPlayer:
	private CGameObject, public Loadable
{
public:
	sf::RectangleShape rect;
	float moveSpeed = 20;

	int Health;
	float Hunger;
	float Thirst;
	bool IsBleeding;

	int Str;
	int Agi;
	int Wis;
	int Def;

	std::vector<std::string> Effects = { "Ayyyyyy","LmaOOOO","ThisWORKSSSS","and its COOL","INDENTSSSSS","WOOOOOOO","YAY","FUNNNNNN","B)","Neat" };
	
	operator GameData()
	{
		GameData dat;
		dat.FileData.GroupID = "PlayerData";
		dat.FileData.m_Data.push_back(Variable(Health));
		dat.FileData.m_Data.push_back(Variable(Hunger));
		dat.FileData.m_Data.push_back(Variable(Thirst));
		dat.FileData.m_Data.push_back(Variable(IsBleeding));

		dat.FileData.m_Data.push_back(Variable(Str));
		dat.FileData.m_Data.push_back(Variable(Agi));
		dat.FileData.m_Data.push_back(Variable(Wis));
		dat.FileData.m_Data.push_back(Variable(Def));

		dat.FileData.m_Groups.push_back(Group(Effects));

		return dat;
	}

	CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col);
	~CPlayer();

	void Movement();
	void ScreenWrap();

	virtual void FixedUpdate();

private:
	int currentStep = 0;
};

