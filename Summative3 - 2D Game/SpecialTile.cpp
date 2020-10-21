#include "SpecialTile.h"

SpecialTile::SpecialTile(sf::Vector2f _pos, std::string _texturepath)
{
	Pos = _pos;
	Texture.loadFromFile(_texturepath);
	Sprite.setTexture(Texture);
	Sprite.setPosition(Pos);
	Sprite.setOrigin(sf::Vector2f(5, 10));
	Sprite.setScale(sf::Vector2f(3, 3));
}



Tree::Tree(sf::Vector2f _pos) :
	SpecialTile(_pos, "Resources/Sprites/Tree.png")
{

}
