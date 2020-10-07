#include "CPlayer.h"

CPlayer::CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col)
{
	rect.setPosition(_pos);
	rect.setSize(_size);
	rect.setFillColor(_col);
}

void CPlayer::FixedUpdate()
{
	Movement();

	ScreenWrap();

	CWindowUtilities::Draw(&rect);

	currentStep++;
}

void CPlayer::Movement()
{
	if (currentStep % 5 == 0) {
		//Horizontal
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rect.move(-moveSpeed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rect.move(moveSpeed, 0);
		}

		//Vertical
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			rect.move(0, -moveSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			rect.move(0, moveSpeed);
		}
	}
}

void CPlayer::ScreenWrap()
{
	//Horizontal
	if (rect.getPosition().x > 1000 - rect.getGlobalBounds().width) rect.setPosition(0, rect.getPosition().y);
	if (rect.getPosition().x < 0) rect.setPosition(1000 - rect.getGlobalBounds().width, rect.getPosition().y);
	
	//Vertical
	if (rect.getPosition().y > 1000 - rect.getGlobalBounds().height) rect.setPosition(rect.getPosition().x, 0);
	if (rect.getPosition().y < 0) rect.setPosition(rect.getPosition().x, 1000 - rect.getGlobalBounds().height);
}
