#include "CEntity.h"
#include "ItemManager.h"

void CEntity::Movement()
{
	if (itemManager->currentlyDragging != nullptr) {
		sf::Vector2f pos =  rect.getPosition();
		sf::Vector2f playerPos = player->rect.getPosition();;

		if (itemManager->currentlyDragging->itemName == "Stick" && sqrt(pow(pos.x - playerPos.x, 2) + pow(pos.y - playerPos.y, 2)) < 200) {
			moveSpeed = -std::abs(moveSpeed);
		}
		else {
			moveSpeed = std::abs(moveSpeed);
		}
	}
	else {
		moveSpeed = std::abs(moveSpeed);
	}

	if (currentStep % 1 == 0) {
		//Horizontal

		if (rect.getPosition().x + 100 > player->rect.getPosition().x)
		{

			rect.move(-moveSpeed, 0);
			if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
			{
				rect.move(moveSpeed, 0);
			}
		}
		
		if (rect.getPosition().x - 100 < player->rect.getPosition().x)
		{
			rect.move(moveSpeed, 0);
			if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
			{
				rect.move(-moveSpeed, 0);
			}
		}

		//Vertical
		if (rect.getPosition().y + 100 > player->rect.getPosition().y)
		{
			rect.move(0, -moveSpeed);
			if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
			{
				rect.move(0, moveSpeed);
			}
		}
		
		if (rect.getPosition().y - 100 < player->rect.getPosition().y)
		{
			rect.move(0, moveSpeed);
			if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
			{
				rect.move(0, -moveSpeed);
			}
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

CEntity::CEntity(EntityType _type, sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col, WorldLayer* _world)
{
	rect.setPosition(_pos);
	rect.setSize(_size);
	rect.setFillColor(_col);
	currentWorld = _world;
	type = _type;
}
