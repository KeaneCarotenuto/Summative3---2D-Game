#include "CItem.h"


bool CItem::operator==(const CItem& _item)
{
	return (typeid(this).name() == typeid(_item).name());
}

void CItem::FixedUpdate()
{
	Draw();
}

CItem::CItem(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name) :
	currentInv(_wind), initialWindow(_wind), initialPos(_pos), itemName(_name)
{
}

void CItem::Draw()
{
}