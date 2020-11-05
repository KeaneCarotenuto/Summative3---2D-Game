#pragma once
#include "CItem.h"
class Tool :
    public CItem
{
public:
    Tool(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, float _durability);
    float Durablity = 100.0f;
};

class Axe :
    public Tool
{
public:
    Axe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _durability);
};

class Pickaxe :
    public Tool
{
public:
    Pickaxe(sf::RenderWindow* _wind, sf::Vector2f _pos, float _durability);
};

