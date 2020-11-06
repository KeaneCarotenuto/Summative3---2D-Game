#include "CEntity.h"
#include "ItemManager.h"

/// <summary>
/// Returns Magnitude of vector
/// </summary>
float GetMag(sf::Vector2f _vec) {
	return sqrt(pow(_vec.x, 2) + pow(_vec.y, 2));
}

/// <summary>
/// Returns a normalised vector
/// </summary>
sf::Vector2f Norm(sf::Vector2f _vec) {
	return _vec * (1 / GetMag(_vec));
}

/// <summary>
/// Movement code for ent
/// </summary>
void CEntity::Movement()
{
	//Only updates every 2 seconds
	if (currentStep % 120 == 0) {
		//Find relevant vectors

		sf::Vector2f toPlayer = player->rect.getPosition() - rect.getPosition();
		float toPlayerMag = GetMag(toPlayer);

		sf::Vector2f toPlayerNormed = Norm(toPlayer) * moveSpeed;

		//If in reach of player move towards
		if (toPlayerMag <= 1000 && toPlayerMag >= 500) {
			velocity = toPlayerNormed;
		}
		//if close to player move randomly
		else if (toPlayerMag < 500) {

			sf::Vector2f randDir = Norm(sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50)) * moveSpeed;

			velocity = -randDir;
			
		}
		//Otherwise move randomly half the time
		else if (currentStep % 2 == 0) {
			sf::Vector2f randDir = Norm(sf::Vector2f(rand() % 100 - 50, rand() % 100 - 50)) * moveSpeed;

			velocity = -randDir;
		}

		//If holding sowrd run away
		if (itemManager->currentlyDragging != nullptr) {
			if (itemManager->currentlyDragging->itemName == "Sword" && toPlayerMag <= 2000) {
				velocity = -toPlayerNormed;
			}
		}
	}
	
	//Translte from screen coords to tile coords
	int x = floor((rect.getPosition() + velocity).x / 20);
	int y = floor((rect.getPosition() + velocity).y / 20);

	//Moves entity if valid
	if (x > 0 && y > 0 && x < 500 && y < 500) {
		if (type != EntityType::Fish || WorldLayer::currentWorld->TerrainTilemap[x][y]->Type == TerrainType::WATER) {
			rect.move(velocity);
		}
	}
}

void CEntity::Update(float _fDeltaTime)
{
	CWindowUtilities::Draw(&rect);
}

void CEntity::FixedUpdate()
{
	Movement();

	currentStep++;
}

CEntity::CEntity(EntityType _type, sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col)
{
	rect.setPosition(_pos);
	rect.setSize(_size);
	rect.setFillColor(_col);
	type = _type;

	switch (type)
	{
	case EntityType::Bird:
		moveSpeed = 3;
		break;
	case EntityType::Fish:
		moveSpeed = 2;
		break;
	default:
		break;
	}
}
