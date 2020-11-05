////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	Stackable
//					Base Class for stackables
//--------------------------------------------------------//
//					BSE20021
//    Author    :	Nerys Thamm
//					Keane Carotenuto
//--------------------------------------------------------//
//    E-mails   :	NerysThamm@gmail.com
//					Keane Carotenuto@gmail.com
//========================================================//
////////////////////////////////////////////////////////////

#pragma once
#include <stack>
#include "CItem.h"

namespace ItemAttributes
{
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
}

