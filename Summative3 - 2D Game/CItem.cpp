#include "CItem.h"


bool CItem::operator==(const CItem& _item)
{
	return (typeid(this).name() == typeid(_item).name());
}

void CItem::FixedUpdate()
{
	
}

void CItem::Update(float _fDeltaTime)
{
	Draw();
}

CItem::CItem(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name, std::string _filepath) :
	currentInv(_wind), initialWindow(_wind), initialPos(_pos), itemName(_name), Loadable("Items/" + _filepath, _name)
{
	if (!texture.loadFromFile(Data->FileData.GetGroupByID("SpriteData").GetDataByID("Path"), sf::IntRect((int)(Data->FileData.GetGroupByID("SpriteData").GetDataByID("TextureIndex")) * 20, 0, 20, 20)))
	{
		std::cout << "[Failed to load CItem texture]   Item Type: " << _name;
	}

	sprite.setTexture(texture);
}

void CItem::Draw()
{
}
