//A Class for clickable buttons

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

