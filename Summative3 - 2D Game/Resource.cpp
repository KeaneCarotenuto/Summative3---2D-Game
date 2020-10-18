#include "Resource.h"

Resource::Resource(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name):
	CItem(_wind, _pos, _name, "Resources/")
{
	sprite.setScale(5, 5);
	sprite.setPosition(_pos);
	
}

Resource::Resource(sf::RenderWindow* _wind, GameData::DataGroup datg, CItem* (*constructor)(sf::RenderWindow* _wind, sf::Vector2f _pos)):
	CItem(_wind, datg.GetGroupByID("initialPos"), datg.GroupID, "Resources/")
{
		for (GameData::DataGroup datgroup : datg.m_Groups)
		{
			if (datgroup.GroupID == datg.GroupID)
			{
				disabledStack.push(constructor(_wind, initialPos));
				disabledStack.top()->bIsEnabled = false;
			}
		}
		sprite.setScale(5, 5);
		sprite.setPosition(initialPos);
}

void Resource::Draw()
{
	currentInv->draw(sprite);

	if (!disabledStack.empty()) {
		stackText.setPosition(sprite.getPosition());
		stackText.setString(std::to_string(disabledStack.size() + 1));

		currentInv->draw(stackText);
	}
}
