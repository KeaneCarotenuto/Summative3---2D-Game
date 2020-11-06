#include "CButton.h"

/// <summary>
/// Constructs Button with given params and stores window to draw to
/// </summary>
/// <param name="_function"> - Function to Ex when Clicked</param>
/// <param name="_string"> - Words on button</param>
/// <param name="_font"></param>
/// <param name="_fontSize"></param>
/// <param name="_tColour"> - colour of text</param>
/// <param name="_style"></param>
/// <param name="_x"> - xpos</param>
/// <param name="_y"> - ypos</param>
/// <param name="_bgColour"> - colour of box</param>
/// <param name="_padding"> - padding around text</param>
/// <param name="_wind"> - window to draw to</param>
/// <returns></returns>
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
