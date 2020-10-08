#include "Resource.h"

Resource::Resource(sf::RenderWindow* _wind, sf::Vector2f _pos, std::string _name):
	CItem(_wind, _pos, _name, "Resources/")
{
	sprite.setScale(5, 5);
	sprite.setPosition(_pos);
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
