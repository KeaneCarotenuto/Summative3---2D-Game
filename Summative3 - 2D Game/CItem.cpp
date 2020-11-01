#include "CItem.h"

std::map <std::string, sf::RenderWindow*> CItem::mapOfWindows;

bool CItem::operator==(const CItem& _item)
{
	return (typeid(this).name() == typeid(_item).name());
}

void CItem::FixedUpdate()
{
	
}

void CItem::Update(float _fDeltaTime)
{
	sf::RenderWindow* worldInv = nullptr;
	std::map < std::string, sf::RenderWindow*>::iterator invWndIt = CItem::mapOfWindows.find("WorldInv");
	if (invWndIt != CItem::mapOfWindows.end()) {
		worldInv = (*invWndIt).second;
	}
	else {
		std::cout << "Failed to Find World Inv\n";
		return;
	}

	if (currentInv == worldInv) {
		if (sqrt(pow(sprite.getPosition().x - CWindowUtilities::ScreenCentre.x, 2) + pow(sprite.getPosition().y - CWindowUtilities::ScreenCentre.y, 2)) <= CWindowUtilities::RenderDistance)
		{
			Draw();
		}
	}
	else {
		Draw();
	}
	
}

void CItem::Draw()
{
}

void CItem::Draw(sf::Vector2f)
{
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
