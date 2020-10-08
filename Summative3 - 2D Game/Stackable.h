#pragma once

#include <stack>

#include "CItem.h"

class Stackable
{
public:
	std::stack<CItem*> disabledStack;

protected:

	sf::Font stackFont;

	sf::Text stackText;

	Stackable() {
		if (!stackFont.loadFromFile("Resources/Fonts/uni.ttf")) {
			std::cout << "[Failed to load CResource font]";
		}
		stackText.setFillColor(sf::Color::White);
		stackText.setFont(stackFont);
	}
};

