////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	Resource
//					Base class for resources
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
#include <iostream>
#include "GameData.h"
#include "CItem.h"
#include "Stackable.h"

class Resource :
	public CItem, public ItemAttributes::Stackable
{
public:
	Resource(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name);
	Resource(sf::RenderWindow* _wind, GameData::DataGroup datg, CItem* (*constructor)(sf::RenderWindow* _wind, GameData::DataGroup _datag));
	

	operator GameData::DataGroup()
	{
		GameData::DataGroup datg;
		
		datg.GroupID = itemName;
		datg.m_Data.push_back(Variable(bIsEnabled));
		datg.m_Groups.push_back(Group(initialPos));
		
		for (int i = 0; i < disabledStack.size(); i++)
		{
			GameData::DataGroup dattemp;
			dattemp.GroupID = itemName;
			dattemp.m_Data.push_back(GameData::Data(false, "bIsEnabled"));
			dattemp.m_Groups.push_back(Group(initialPos));
			datg.m_Groups.push_back(dattemp);
		}
		
		return datg;
	}

	virtual void Draw();
	
};

