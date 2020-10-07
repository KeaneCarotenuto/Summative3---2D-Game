#include "CItem.h"

CItem::CItem(ItemType _type, sf::RenderWindow* _main, sf::RenderWindow* _inv, bool _isInInv):
	type(_type), main(_main), inv(_inv), isInInv(_isInInv)
{
	if (!texture.loadFromFile("Resources/Sprites/Sprites.png", sf::IntRect((int)type *20, 0, 20, 20)))
	{
		std::cout << "[Failed to load CItem texture]   Item Type: " << (int)type;
	}

	sprite.setTexture(texture);

	sprite.setPosition(-50, -50);
	sprite.setScale(5, 5);

}


bool mouseInMain = true;
bool mousInInv = true;
void CItem::FixedUpdate()
{
	

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		bool scaleUpdated = true;

		
		if (main->getPosition().x <= sf::Mouse::getPosition().x && main->getPosition().x + main->getSize().x >= sf::Mouse::getPosition().x 
			&& main->getPosition().y <= sf::Mouse::getPosition().y && main->getPosition().y + main->getSize().y >= sf::Mouse::getPosition().y) {
			mouseInMain = true;
		}
		else
		{
			mouseInMain = false;
		}
		
		if (inv->getPosition().x <= sf::Mouse::getPosition().x && inv->getPosition().x + inv->getSize().x >= sf::Mouse::getPosition().x
			&& inv->getPosition().y <= sf::Mouse::getPosition().y && inv->getPosition().y + inv->getSize().y >= sf::Mouse::getPosition().y) {
			mousInInv = true;
		}
		else
		{
			mousInInv = false;
		}

		if (mousInInv && beingDragged){
			isInInv = true;
		}
		else if (mouseInMain && beingDragged) {
			isInInv = false;
		}

		if (!beingDragged && sprite.getGlobalBounds().contains((isInInv ? inv : main)->mapPixelToCoords(sf::Mouse::getPosition(*(isInInv ? inv : main))))) {
			beingDragged = true;
			initialPos = sprite.getPosition();
		}

		if (beingDragged) {
			std::cout << mouseInMain << ", " << mousInInv << "\n";
			sprite.setPosition(	(isInInv ? inv : main)->mapPixelToCoords(sf::Mouse::getPosition(*(isInInv ? inv : main))).x - sprite.getGlobalBounds().width/2,
								(isInInv ? inv : main)->mapPixelToCoords(sf::Mouse::getPosition(*(isInInv ? inv : main))).y - sprite.getGlobalBounds().height/2);
		}
	}
	else {
		if (mouseInMain == false && mousInInv == false) {
			sprite.setPosition(initialPos);
		}


		beingDragged = false;
		float scale = (isInInv ? (sprite.getScale().x == 5 ? 1 : 5) : (sprite.getScale().x == 5 ? 1 / 5 : 1));
		//sprite.scale(scale, scale);
	}

	sprite.setTexture(texture);
	(isInInv ? inv : main)->draw(sprite);
}
