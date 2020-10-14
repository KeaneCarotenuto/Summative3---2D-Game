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
		dat.FileData.m_Data.push_back(GameData::Data(Health, "Health"));
		dat.FileData.m_Data.push_back(GameData::Data(Hunger, "Hunger"));
		dat.FileData.m_Data.push_back(GameData::Data(Thirst, "Thirst"));
		dat.FileData.m_Data.push_back(GameData::Data(IsBleeding, "IsBleeding"));

		dat.FileData.m_Data.push_back(GameData::Data(Str, "Str"));
		dat.FileData.m_Data.push_back(GameData::Data(Agi, "Agi"));
		dat.FileData.m_Data.push_back(GameData::Data(Wis, "Wis"));
		dat.FileData.m_Data.push_back(GameData::Data(Def, "Def"));

		dat.FileData.m_Groups.push_back(Effects);

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

