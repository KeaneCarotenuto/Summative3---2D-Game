#include "CButton.h"

CButton::CButton(void(*_function)(), std::string _string, sf::Font _font, int _fontSize, sf::Color _tColour, sf::Text::Style _style, float _x, float _y, sf::Color _bgColour, float _padding, sf::RenderWindow* _wind)
{
    targetWindow = _wind;

    //Text
    text = new sf::Text();
    text->setString(_string);
    text->setCharacterSize(_fontSize);
    text->setFillColor(_tColour);
    text->setStyle(_style);
    text->setFont(_font);

    //Middle of button
    text->setPosition(100 - (text->getGlobalBounds().width) / 2, _y);

    //Button rect
    rect = new sf::RectangleShape();
    rect->setPosition(0, text->getGlobalBounds().top - _padding);
    rect->setSize(sf::Vector2f(200, 30));
    rect->setFillColor(_bgColour);

    function = _function;
}

CButton::~CButton()
{
}
