#include "Lumber.h"

Lumber::Lumber(LumberType _type, sf::RenderWindow* _wind, sf::Vector2f _pos):
    type(_type), Resource(_wind, _pos, ((int)_type == 0 ? "Stick" : "Log" ))
{

    if (!texture.loadFromFile("Resources/Sprites/Sprites.png", sf::IntRect((int)type * 20, 0, 20, 20)))
    {
        std::cout << "[Failed to load CItem texture]   Item Type: " << (int)type;
    }

    sprite.setTexture(texture);
    
}
