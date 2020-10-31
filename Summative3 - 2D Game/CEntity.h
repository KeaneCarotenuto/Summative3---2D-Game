#pragma once
#include <SFML/Graphics.hpp>
#include "EasySFML.h"
#include "WorldLayer.h"
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
	WorldLayer* currentWorld;
	int currentStep = 0;
	EntityType type;

	sf::Vector2f velocity;

	CPlayer* player;

	ItemManager* itemManager;

	void Movement();

	virtual void Update(float _fDeltaTime);
	virtual void FixedUpdate();

	CEntity(EntityType _type ,sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col, WorldLayer* _world);

};

