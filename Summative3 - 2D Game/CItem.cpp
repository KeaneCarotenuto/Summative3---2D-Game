#include "CItem.h"

CItem::CItem(ItemType _type, sf::RenderWindow* _wind, sf::Vector2f _pos):
	type(_type), currentInv(_wind), initialWindow(_wind), initialPos(_pos)
{
	if (!texture.loadFromFile("Resources/Sprites/Sprites.png", sf::IntRect((int)type *20, 0, 20, 20)))
	{
		std::cout << "[Failed to load CItem texture]   Item Type: " << (int)type;
	}

	sprite.setTexture(texture);

	sprite.setPosition(_pos);
	sprite.setScale(5, 5);

}

CItem::CItem()
{
}

void CItem::FixedUpdate()
{
	Draw();
}

void CItem::Draw()
{
}
