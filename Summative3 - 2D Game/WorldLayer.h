#pragma once
#include "SpecialTile.h"
#include "GameEntity.h"
#include "EasySFML.h"
#include <vector>
#include <SFML/Graphics.hpp>
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
	TerrainTile* TerrainTilemap[50][50];
	SpecialTile* SpecialTilemap[50][50];
	WallTile* WallTilemap[50][50];
	int LightMap[50][50];
	//TODO: Navigation 
	
	int Altitude;
	
	WorldLayer();
	WorldLayer(WorldLayer* _Above, WorldLayer* _Below, int _altitude);
	WorldLayer* loadAboveLayer();
	WorldLayer* loadBelowLayer();
	void resetLightMap();
	void renderLightMap(bool isCircular);
	void addPointLight(int _X, int _Y, int _intensity);
	
private:
	WorldLayer* m_pAboveLayer;
	WorldLayer* m_pBelowLayer;
	std::vector<GameEntity> Entities;
	void populateTileMaps();
	void renderTileMaps();
	

	sf::Texture m_TerrainTexture;
	sf::VertexArray m_TerrainVertices;
	sf::Texture m_WallTexture;
	sf::VertexArray m_WallVertices;
	sf::VertexArray m_SpecialVertices;
	sf::Texture m_LightLevelTexture;
	sf::VertexArray m_LightLevelVertices;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

