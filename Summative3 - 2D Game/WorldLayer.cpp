#include "WorldLayer.h"

TerrainTile::TerrainTile(TerrainType _type, int _Xpos, int _Ypos)
{
	Type = _type;
	Collider.setSize(sf::Vector2f(20, 20));
	Collider.setPosition(sf::Vector2f((_Xpos * 20), (_Ypos * 20)));
	
	
	
}

WallTile::WallTile(WallType _type, int _Xpos, int _Ypos)
{
	Type = _type;
	Collider.setSize(sf::Vector2f(20, 20));
	Collider.setPosition(sf::Vector2f((_Xpos * 20), (_Ypos * 20)));
	
}

WallTile::WallTile(int _Xpos, int _Ypos)
{
	Type = WallType::INVIS;
	Collider.setSize(sf::Vector2f(20, 20));
	Collider.setPosition(sf::Vector2f((_Xpos * 20), (_Ypos * 20)));
}

WorldLayer::WorldLayer()
{
	Altitude = 1;
	//Todo: Generate TileMaps
	populateTileMaps();
	m_pAboveLayer = nullptr;
	m_pBelowLayer = nullptr;
	//Todo: Spawn entities
}

WorldLayer::WorldLayer(WorldLayer* _Above, WorldLayer* _Below, int _altitude)
{
	Altitude = _altitude;
	//Todo: Generate TileMaps
	populateTileMaps();
	m_pAboveLayer = _Above;
	m_pBelowLayer = _Below;
	//Todo: Spawn entities
}

WorldLayer* WorldLayer::loadAboveLayer()
{
	if (m_pAboveLayer == nullptr)
	{
		m_pAboveLayer = new WorldLayer(nullptr, this, Altitude + 1);
	}
	return m_pAboveLayer;
}

WorldLayer* WorldLayer::loadBelowLayer()
{
	if (m_pBelowLayer == nullptr)
	{
		m_pBelowLayer = new WorldLayer(this, nullptr, Altitude - 1);
	}
	return m_pBelowLayer;
}



void WorldLayer::populateTileMaps()
{
	int map[50][50];

	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			if (((i - 4) < j) && ((i + 4) > j))
			{
				map[i][j] = 5;
			}
			else if (((i - (6+(rand()%4+1))) < j) && ((i + (6 + (rand() % 4 + 1))) > j))
			{
				map[i][j] = 1;
			}
			else
			{
				switch (rand() % 3 + 1)
				{
				case 1:
					map[i][j] = 0;
					break;
				case 2:
					map[i][j] = 2;
					break;
				case 3:
					map[i][j] = 4;
					break;
				default:
					map[i][j] = 6;
					break;
				}
				
			}
		}
	}
	
	//TESTING CODE
	for (size_t i = 0; i < 50; i++)
	{
		for (size_t j = 0; j < 50; j++)
		{
			TerrainTilemap[i][j] = new TerrainTile((TerrainType)(map[i][j]), i, j);
			SpecialTilemap[i][j] = nullptr;
			WallTilemap[i][j] = nullptr;
		}
	}
	renderTileMaps();
}

void WorldLayer::renderTileMaps()
{
	m_TerrainTexture.loadFromFile("Resources/Test/tileset.png");
	m_TerrainVertices.setPrimitiveType(sf::Quads);
	m_TerrainVertices.resize(50 * 50 * 4);

	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			sf::Vertex* quad = &m_TerrainVertices[(i + j * 50) * 4];

			quad[0].position = sf::Vector2f(i * 20, j * 20);
			quad[1].position = sf::Vector2f((i+1) * 20, j * 20);
			quad[2].position = sf::Vector2f((i+1) * 20, (j+1) * 20);
			quad[3].position = sf::Vector2f(i * 20, (j+1) * 20);

			quad[0].texCoords = sf::Vector2f((int)(TerrainTilemap[i][j]->Type)*10, 0);
			quad[1].texCoords = sf::Vector2f(((int)(TerrainTilemap[i][j]->Type) * 10) + 10, 0);
			quad[2].texCoords = sf::Vector2f(((int)(TerrainTilemap[i][j]->Type) * 10) + 10, 10);
			quad[3].texCoords = sf::Vector2f((int)(TerrainTilemap[i][j]->Type) * 10, 10);
		}
	}
}

void WorldLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.transform *= getTransform();

	states.texture = &m_TerrainTexture;

	target.draw(m_TerrainVertices, states);
}
