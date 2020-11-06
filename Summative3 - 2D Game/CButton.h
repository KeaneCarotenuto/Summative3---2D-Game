////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	Button
//					A Clickable button with a function
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
#include <SFML/Graphics.hpp>

class CButton
{
public:
	CButton(void(*_function)(), std::string _string, sf::Font _font, int _fontSize, sf::Color _tColour, sf::Text::Style _style, float _x, float _y, sf::Color _bgColour, float _padding, sf::RenderWindow* _wind);
	~CButton();
	sf::RectangleShape* rect;
	sf::Text* text;
	sf::RenderWindow* targetWindow;
	void(*function)();
};

