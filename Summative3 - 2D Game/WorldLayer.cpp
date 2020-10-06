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
	noise::module::Perlin Noise;
	Noise.SetSeed(time(0));
	double val;
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			
			val = Noise.GetValue(i*(2.0f/50.0f), j * (2.0f / 50.0f), 0.5);
			val -= (0.1 * ((abs(25 - i) + abs(25 - j))-20));
			if (val < -0.9)
			{
				map[i][j] = 0;
			}
			else if (val < -0.7)
			{
				map[i][j] = 1;
			}
			else if (val < -0.4)
			{
				map[i][j] = 2;
			}
			else if (val < 0.7)
			{
				map[i][j] = 3;
			}
			else if (val < 0.8)
			{
				map[i][j] = 4;
			}
			else if (val < 1.3)
			{
				map[i][j] = 5;
			}
			else
			{
				map[i][j] = 6;
				
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
			if (map[i][j] == 6)
			{
				WallTilemap[i][j] = new WallTile(WallType::ROCK, i, j);
			}
			else
			{
				WallTilemap[i][j] = nullptr;
			}
		}
	}
	renderTileMaps();
	
}

void WorldLayer::renderTileMaps()
{
	m_TerrainTexture.loadFromFile("Resources/Test/TerrainTileset.png");
	m_TerrainVertices.setPrimitiveType(sf::Quads);
	m_TerrainVertices.resize(50 * 50 * 4);
	m_WallTexture.loadFromFile("Resources/Test/WallTileset.png");
	m_WallVertices.setPrimitiveType(sf::Quads);
	m_WallVertices.resize(50 * 50 * 4);

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

			if (WallTilemap[i][j] != nullptr)
			{
				sf::Vertex* quad2 = &m_WallVertices[(i + j * 50) * 4];

				quad2[0].position = sf::Vector2f(i * 20, j * 20);
				quad2[1].position = sf::Vector2f((i + 1) * 20, j * 20);
				quad2[2].position = sf::Vector2f((i + 1) * 20, (j + 1) * 20);
				quad2[3].position = sf::Vector2f(i * 20, (j + 1) * 20);

				quad2[0].texCoords = sf::Vector2f((int)(WallTilemap[i][j]->Type) * 10, 0);
				quad2[1].texCoords = sf::Vector2f(((int)(WallTilemap[i][j]->Type) * 10) + 10, 0);
				quad2[2].texCoords = sf::Vector2f(((int)(WallTilemap[i][j]->Type) * 10) + 10, 10);
				quad2[3].texCoords = sf::Vector2f((int)(WallTilemap[i][j]->Type) * 10, 10);
			}
		}
	}
}

void WorldLayer::resetLightMap()
{
	int iGlobalLightLevel;
	if (Altitude > 0)
	{
		iGlobalLightLevel = 0;
	}
	else
	{
		iGlobalLightLevel = 0;
	}
	for (int i = 0; i < 50; i++)
	{
		for (int j = 0; j < 50; j++)
		{
			if (WallTilemap[i][j] == nullptr)
			{
				LightMap[i][j] = iGlobalLightLevel;
			}
			else
			{
				LightMap[i][j] = 0;
			}
		}
	}
}

void WorldLayer::renderLightMap(bool isCircular)
{
	if (isCircular) {
		int tempMap[50][50];

		for (int x = 0; x < 50; x++) {
			for (int y = 0; y < 50; y++) {

				if (LightMap[x][y] > 0) {

					tempMap[x][y] = LightMap[x][y];
					for (int tx = 0; tx < 50; tx++) {
						for (int ty = 0; ty < 50; ty++) {
							int l = 10 - std::abs(std::sqrt((ty - y) * (ty - y) + (tx - x) * (tx - x)));
							l = (l > 9 ? 9 : (l < 0 ? 0 : l));
							tempMap[tx][ty] = (l > tempMap[tx][ty] ? l : tempMap[tx][ty]);
						}
					}

				}

			}
		}

		for (int x = 0; x < 50; x++) {
			for (int y = 0; y < 50; y++) {
				LightMap[x][y] = tempMap[x][y];
			}
		}
	}
	else {
		for (int x = 10; x > 0; x--)
		{
			for (int i = 0; i < 50; i++)
			{
				for (int j = 0; j < 50; j++)
				{
					if (LightMap[i][j] == x)
					{
						if (i > 0)
						{
							if (LightMap[i - 1][j] < x)
							{
								LightMap[i - 1][j] = x - 1;
							}
						}
						if (i < 49)
						{

							if (LightMap[i + 1][j] < x)
							{
								LightMap[i + 1][j] = x - 1;
							}

						}
						if (i > 0 && j > 0)
						{
							if (LightMap[i - 1][j - 1] < x)
							{
								LightMap[i - 1][j - 1] = x - 1;
							}
						}
						if (i < 49 && j < 49)
						{
							if (LightMap[i + 1][j + 1] < x)
							{
								LightMap[i + 1][j + 1] = x - 1;
							}
						}
						if (j > 0)
						{

							if (LightMap[i][j - 1] < x)
							{
								LightMap[i][j - 1] = x - 1;
							}



						}
						if (j < 49)
						{

							if (LightMap[i][j + 1] < x)
							{
								LightMap[i][j + 1] = x - 1;
							}

						}
						if (i > 0 && j < 49)
						{
							if (LightMap[i - 1][j + 1] < x)
							{
								LightMap[i - 1][j + 1] = x - 1;
							}
						}
						if (i < 49 && j > 0)
						{
							if (LightMap[i + 1][j - 1] < x)
							{
								LightMap[i + 1][j - 1] = x - 1;
							}
						}
					}
				}
			}
		}
	}
	
	
	
	
	m_LightLevelTexture.loadFromFile("Resources/Test/LightLevels.png");
	m_LightLevelVertices.setPrimitiveType(sf::Quads);
	m_LightLevelVertices.resize(50 * 50 * 4);

	for (int i = 0; i < 50; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			sf::Vertex* quad = &m_LightLevelVertices[(i + j * 50) * 4];

			quad[0].position = sf::Vector2f(i * 20, j * 20);
			quad[1].position = sf::Vector2f((i + 1) * 20, j * 20);
			quad[2].position = sf::Vector2f((i + 1) * 20, (j + 1) * 20);
			quad[3].position = sf::Vector2f(i * 20, (j + 1) * 20);

			quad[0].texCoords = sf::Vector2f(LightMap[i][j] * 10, 0);
			quad[1].texCoords = sf::Vector2f((LightMap[i][j] * 10) + 10, 0);
			quad[2].texCoords = sf::Vector2f((LightMap[i][j] * 10) + 10, 10);
			quad[3].texCoords = sf::Vector2f((LightMap[i][j] * 10), 10);

			
		}
	}
}

void WorldLayer::addPointLight(int _X, int _Y, int _intensity)
{
	LightMap[_X][_Y] = _intensity;
}

void WorldLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.transform *= getTransform();

	states.texture = &m_TerrainTexture;
	target.draw(m_TerrainVertices, states);

	states.texture = &m_WallTexture;
	target.draw(m_WallVertices, states);

	states.texture = &m_LightLevelTexture;
	target.draw(m_LightLevelVertices, states);
}
