#include "SpecialTile.h"

SpecialTile::SpecialTile(SpecialType _type ,sf::Vector2f _pos, std::string _texturepath)
{
	health = 100;
	type = _type;
	Pos = _pos;
	Texture.loadFromFile(_texturepath);
	Sprite.setTexture(Texture);
	Sprite.setPosition(Pos);
	Sprite.setOrigin(sf::Vector2f(9, 15));
	Sprite.setScale(sf::Vector2f(6, 6));
}



Tree::Tree(sf::Vector2f _pos) :
	SpecialTile(SpecialType::Tree, _pos, "Resources/Sprites/Tree.png")
{

}

Boulder::Boulder(sf::Vector2f _pos) :
	SpecialTile(SpecialType::Boulder, _pos, "Resources/Sprites/Boulder.png")
{
}
