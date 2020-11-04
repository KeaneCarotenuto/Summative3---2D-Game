#pragma once
#include "SpecialTile.h"
#include "EasySFML.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include <noise/module/perlin.h>
#include <noise/noise.h>

enum class TerrainType
{
	WATER,
	SAND,
	MUD,
	GRASS,
	DIRT,
	ROCK,
	HOLE
};
enum class WallType
{
	DIRT,
	ROCK,
	BRICK,
	INVIS
};
struct TerrainTile
{
	TerrainType Type;
	sf::RectangleShape Collider;
	TerrainTile(TerrainType _type, int _Xpos, int _Ypos);
	
};
struct WallTile
{
	WallType Type;
	sf::RectangleShape Collider;
	WallTile(WallType _type, int _Xpos, int _Ypos);
	
	WallTile(int _Xpos, int _Ypos);
	
};

class WorldLayer
	: public sf::Drawable, public sf::Transformable
{
public:
	static const int width = 500;
	static const int height = 500;

	TerrainTile* TerrainTilemap[width][height];
	SpecialTile* SpecialTilemap[width][height];
	WallTile* WallTilemap[width][height];
	int LightMap[width][height];
	int seed;
	//TODO: Navigation
	
	int Altitude;
	
	WorldLayer(int _seed);
	WorldLayer(int _seed, WorldLayer*&_Above, WorldLayer*&_Below, int _altitude);
	void resetLightMap();
	void renderLightMap();
	void addPointLight(int _X, int _Y, int _intensity);
	void renderTileMaps();
	sf::Vector2f GetFirstSandTilePos();
	bool CheckCollision(sf::Vector2f _nextPos);
	void DrawSpecial();
	
private:
	void populateTileMaps();
	
	

	sf::Texture m_TerrainTexture;
	sf::VertexArray m_TerrainVertices;
	sf::Texture m_WallTexture;
	sf::VertexArray m_WallVertices;
	sf::VertexArray m_SpecialVertices;
	sf::Texture m_LightLevelTexture;
	sf::VertexArray m_LightLevelVertices;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

