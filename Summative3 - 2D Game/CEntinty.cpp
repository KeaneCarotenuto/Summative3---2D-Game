#include "CEntity.h"
#include "ItemManager.h"

void CEntity::Movement()
{
	velocity = player->rect.getPosition() - rect.getPosition();

	float mag = sqrt(pow(rect.getPosition().x - player->rect.getPosition().x, 2) + pow(rect.getPosition().y - player->rect.getPosition().y, 2));
	
	velocity *= 1/mag * moveSpeed;
	

	int x = floor((rect.getPosition() + velocity).x / 20);
	int y = floor((rect.getPosition() + velocity).y / 20);

	if (x > 0 && y > 0 && x < 500 && y < 500) {
		if (type != EntityType::Fish || Globals::currentWorld->TerrainTilemap[x][y]->Type == TerrainType::WATER) {
			rect.move(velocity);
		}
	}

	//if (itemManager->currentlyDragging != nullptr) {
	//	sf::Vector2f pos =  rect.getPosition();
	//	sf::Vector2f playerPos = player->rect.getPosition();;

	//	if (itemManager->currentlyDragging->itemName == "Stick" && sqrt(pow(pos.x - playerPos.x, 2) + pow(pos.y - playerPos.y, 2)) < 200) {
	//		moveSpeed = -std::abs(moveSpeed);
	//	}
	//	else {
	//		moveSpeed = std::abs(moveSpeed);
	//	}
	//}
	//else {
	//	moveSpeed = std::abs(moveSpeed);
	//}

	//if (currentStep % 1 == 0) {
	//	//Horizontal

	//	int x = floor(rect.getPosition().x / 20);
	//	int y = floor(rect.getPosition().y / 20);

	//	if (rect.getPosition().x + 100 > player->rect.getPosition().x)
	//	{

	//		rect.move(-moveSpeed, 0);

	//		if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
	//		{
	//			if (type == EntityType::Fish && currentWorld->TerrainTilemap[x][y]->Type == TerrainType::WATER) {

	//			}
	//			else rect.move(moveSpeed, 0);
	//		}
	//	}
	//	
	//	if (rect.getPosition().x - 100 < player->rect.getPosition().x)
	//	{
	//		rect.move(moveSpeed, 0);
	//		if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
	//		{
	//			if (type == EntityType::Fish && currentWorld->TerrainTilemap[x][y]->Type == TerrainType::WATER) {

	//			}
	//			else rect.move(-moveSpeed, 0);
	//		}
	//	}

	//	//Vertical
	//	if (rect.getPosition().y + 100 > player->rect.getPosition().y)
	//	{
	//		rect.move(0, -moveSpeed);
	//		if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
	//		{
	//			if (type == EntityType::Fish && currentWorld->TerrainTilemap[x][y]->Type == TerrainType::WATER) {

	//			}
	//			else rect.move(0, moveSpeed);
	//		}
	//	}
	//	
	//	if (rect.getPosition().y - 100 < player->rect.getPosition().y)
	//	{
	//		rect.move(0, moveSpeed);
	//		if (currentWorld->CheckCollision(rect.getPosition()) || type != EntityType::Bird)
	//		{
	//			if (type == EntityType::Fish && currentWorld->TerrainTilemap[x][y]->Type == TerrainType::WATER) {

	//			}
	//			else rect.move(0, -moveSpeed);
	//		}
	//	}
	//}
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
