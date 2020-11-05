#pragma once
#include "CItem.h"

enum class ToolType
{
	Pickaxe,
	Axe,
	Sword
};

class Tool :
    public CItem
{
public:
    Tool(ToolType _type, sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability);
	Tool(sf::RenderWindow* _wind, GameData::DataGroup datg, ToolType _type);
    float Durability;
	sf::Text DurabilityText;
	sf::Font Font;
	ToolType type;
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
	static CItem* Axe(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Tool(ToolType::Axe, _wind, _pos, "Axe", 100); }
	static CItem* Pickaxe(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Tool(ToolType::Pickaxe, _wind, _pos, "Pickaxe", 100 ); }
	static CItem* Sword(sf::RenderWindow* _wind, GameData::DataGroup _datag) { return new Tool(_wind, _datag, ToolType::Sword); }
	static CItem* Sword(sf::RenderWindow* _wind, sf::Vector2f _pos) { return new Tool(ToolType::Sword, _wind, _pos, "Sword", 100); }
	
};



