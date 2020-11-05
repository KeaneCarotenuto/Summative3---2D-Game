#pragma once
#include "CItem.h"

enum class ToolType
{
	Pickaxe,
	Axe
};

class Tool :
    public CItem
{
public:
    Tool(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability, ToolType _type);
	Tool(sf::RenderWindow* _wind, GameData::DataGroup datg, ToolType _type);
    float Durability;
	ToolType Type;
	operator GameData::DataGroup()
	{
		GameData::DataGroup datg;

		datg.GroupID = itemName;
		datg.m_Data.push_back(Variable(Durability));
		datg.m_Groups.push_back(Group(initialPos));

		return datg;
	}
	virtual void Draw();

	static CItem* Axe(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Tool(_wind, _datag, ToolType::Axe); }
	static CItem* Pickaxe(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Tool(_wind, _datag, ToolType::Pickaxe); }
	static CItem* Axe(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Tool(_wind, _pos, "Axe", 100, ToolType::Axe); }
	static CItem* Pickaxe(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Tool(_wind, _pos, "Pickaxe", 100, ToolType::Pickaxe); }
};



