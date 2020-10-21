#include "WorldLayer.h"

TerrainTile::TerrainTile(TerrainType _type, int _Xpos, int _Ypos)
{
	Type = _type;
	Collider.setSize(sf::Vector2f(20, 20));
	Collider.setPosition(sf::Vector2f((float)(_Xpos * 20), (float)(_Ypos * 20)));
	
	
	
}

WallTile::WallTile(WallType _type, int _Xpos, int _Ypos)
{
	Type = _type;
	Collider.setSize(sf::Vector2f(20, 20));
	Collider.setPosition(sf::Vector2f((float)(_Xpos * 20), (float)(_Ypos * 20)));
	
}

WallTile::WallTile(int _Xpos, int _Ypos)
{
	Type = WallType::INVIS;
	Collider.setSize(sf::Vector2f(20, 20));
	Collider.setPosition(sf::Vector2f((float)(_Xpos * 20), (float)(_Ypos * 20)));
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
	int map[500][500];
	noise::module::Perlin Noise;
	Noise.SetSeed((int)time(0));
	double val;
	for (int i = 0; i < 500; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			
			val = Noise.GetValue((double)(i)*(2.0f/500.0f), (double)(j) * (2.0f / 500.0f), 0.5);
			val -= 0.006 * std::abs(std::sqrt((i - 250) * (i - 250) + (j - 250) * (j - 250)));
			if (val < -0.9)
			{
				map[i][j] = 0;
			}
			else if (val < -0.8)
			{
				map[i][j] = 1;
			}
			else if (val < -0.75)
			{
				map[i][j] = 2;
			}
			else if (val < -0.1)
			{
				map[i][j] = 3;
			}
			else if (val < 0)
			{
				map[i][j] = 4;
			}
			else if (val < 0.2)
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
	for (size_t i = 0; i < 500; i++)
	{
		for (size_t j = 0; j < 500; j++)
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
			if (map[i][j] == 4 && rand()%3 == 1 )
			{

			}
			else if ((map[i][j] == 5 || map[i][j] == 3) && rand() % 6 == 1)
			{
				SpecialTilemap[i][j] = new Tree(sf::Vector2f(i * 20, j * 20));
			}
			else
			{
				SpecialTilemap[i][j] = nullptr;
			}
		}
	}
	renderTileMaps();
	
}

void WorldLayer::renderTileMaps()
{

	float xIndex = floorf(CWindowUtilities::ScreenCentre.x / 20);
	float yIndex = floorf(CWindowUtilities::ScreenCentre.y / 20);

	float xIndexMin = xIndex - 50;
	float yIndexMin = yIndex - 40;
	float xIndexMax = xIndex + 50;
	float yIndexMax = yIndex + 40;
	if (xIndexMin <= 0)
	{
		xIndexMin = 0;
	}
	if (yIndexMin <= 0)
	{
		yIndexMin = 0;
	}
	if (xIndexMax >= 500)
	{
		xIndexMax = 499;
	}
	if (yIndexMax >= 499)
	{
		yIndexMax = 499;
	}

	m_TerrainTexture.loadFromFile("Resources/Test/TerrainTileset.png");
	m_TerrainVertices.setPrimitiveType(sf::Quads);
	m_TerrainVertices.resize(500 * 500 * 4);
	m_WallTexture.loadFromFile("Resources/Test/WallTileset.png");
	m_WallVertices.setPrimitiveType(sf::Quads);
	m_WallVertices.resize(500 * 500 * 4);


	for (int i = xIndexMin; i < xIndexMax; ++i)
	{
		for (int j = yIndexMin; j < yIndexMax; ++j)
		{
			sf::Vertex* quad = &m_TerrainVertices[(i + j * 500) * 4];

			quad[0].position = sf::Vector2f((float)i * 20, (float)j * 20);
			quad[1].position = sf::Vector2f((float)(i+1) * 20, (float)j * 20);
			quad[2].position = sf::Vector2f((float)(i+1) * 20, (float)(j+1) * 20);
			quad[3].position = sf::Vector2f((float)i * 20, (float)(j+1) * 20);

			quad[0].texCoords = sf::Vector2f((float)(TerrainTilemap[i][j]->Type)*10, 0);
			quad[1].texCoords = sf::Vector2f(((float)(TerrainTilemap[i][j]->Type) * 10) + 10, 0);
			quad[2].texCoords = sf::Vector2f(((float)(TerrainTilemap[i][j]->Type) * 10) + 10, 10);
			quad[3].texCoords = sf::Vector2f((float)(TerrainTilemap[i][j]->Type) * 10, 10);

			if (WallTilemap[i][j] != nullptr)
			{
				sf::Vertex* quad2 = &m_WallVertices[(i + j * 500) * 4];

				quad2[0].position = sf::Vector2f((float)i * 20, (float)j * 20);
				quad2[1].position = sf::Vector2f((float)(i + 1) * 20, (float)j * 20);
				quad2[2].position = sf::Vector2f((float)(i + 1) * 20, (float)(j + 1) * 20);
				quad2[3].position = sf::Vector2f((float)i * 20, (float)(j + 1) * 20);

				quad2[0].texCoords = sf::Vector2f((float)(WallTilemap[i][j]->Type) * 10, 0);
				quad2[1].texCoords = sf::Vector2f(((float)(WallTilemap[i][j]->Type) * 10) + 10, 0);
				quad2[2].texCoords = sf::Vector2f(((float)(WallTilemap[i][j]->Type) * 10) + 10, 10);
				quad2[3].texCoords = sf::Vector2f((float)(WallTilemap[i][j]->Type) * 10, 10);
			}
		}
	}
}

sf::Vector2f WorldLayer::GetFirstSandTilePos()
{
	for (int i = 0; i < 500; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			if (TerrainTilemap[i][j]->Type == TerrainType::SAND)
			{
				return sf::Vector2f(i * 20, j * 20);
			}
		}
	}
	return sf::Vector2f(250, 250);
}

bool WorldLayer::CheckCollision(sf::Vector2f _nextPos)
{
	int x = floor(_nextPos.x / 20);
	int y = floor(_nextPos.y / 20);


	return ((SpecialTilemap[x][y] != nullptr)||(WallTilemap[x][y] != nullptr));
}

void WorldLayer::resetLightMap()
{
	int iGlobalLightLevel;
	if (Altitude > 0)
	{
		iGlobalLightLevel = 5;
	}
	else
	{
		iGlobalLightLevel = 0;
	}
	for (int i = 0; i < 500; i++)
	{
		for (int j = 0; j < 500; j++)
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

void WorldLayer::renderLightMap()
{
	//Circular Light
	/*if (false) {
		int tempMap[50][50];

		for (int x = 0; x < 500; x++) {
			for (int y = 0; y < 500; y++) {

				if (LightMap[x][y] > 0) {

					tempMap[x][y] = LightMap[x][y];
					for (int tx = 0; tx < 50; tx++) {
						for (int ty = 0; ty < 50; ty++) {
							int l = LightMap[x][y] - std::abs(std::sqrt((ty - y) * (ty - y) + (tx - x) * (tx - x)));
							l = (l > 9 ? 9 : (l < 0 ? 0 : l));
							tempMap[tx][ty] = (l > tempMap[tx][ty] ? l : tempMap[tx][ty]);
						}
					}

				}

			}
		}

		for (int x = 0; x < 500; x++) {
			for (int y = 0; y < 500; y++) {
				LightMap[x][y] = tempMap[x][y];
			}
		}
	}*/


	for (int x = 9; x > 0; x--)
	{
		for (int i = 0; i < 500; i++)
		{
			for (int j = 0; j < 500; j++)
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
					if (i < 499)
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
					if (i < 499 && j < 499)
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
					if (j < 499)
					{

						if (LightMap[i][j + 1] < x)
						{
							LightMap[i][j + 1] = x - 1;
						}

					}
					if (i > 0 && j < 499)
					{
						if (LightMap[i - 1][j + 1] < x)
						{
							LightMap[i - 1][j + 1] = x - 1;
						}
					}
					if (i < 499 && j > 0)
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
	
	
	
	
	m_LightLevelTexture.loadFromFile("Resources/Test/LightLevels.png");
	m_LightLevelVertices.setPrimitiveType(sf::Quads);
	m_LightLevelVertices.resize(500 * 500 * 4);

	for (int i = 0; i < 500; ++i)
	{
		for (int j = 0; j < 500; ++j)
		{
			sf::Vertex* quad = &m_LightLevelVertices[(i + j * 500) * 4];

			quad[0].position = sf::Vector2f((float) i * 20, (float)j * 20);
			quad[1].position = sf::Vector2f((float) (i + 1) * 20, (float)j * 20);
			quad[2].position = sf::Vector2f((float) (i + 1) * 20, (float)(j + 1) * 20);
			quad[3].position = sf::Vector2f((float) i * 20, (float)(j + 1) * 20);

			quad[0].texCoords = sf::Vector2f((float)(LightMap[i][j] * 10), 0);
			quad[1].texCoords = sf::Vector2f((float)(LightMap[i][j] * 10) + 10, 0);
			quad[2].texCoords = sf::Vector2f((float) (LightMap[i][j] * 10) + 10, (float)10);
			quad[3].texCoords = sf::Vector2f((float) (LightMap[i][j] * 10), (float)10);

			
		}
	}
}

void WorldLayer::addPointLight(int _X, int _Y, int _intensity)
{
	if (WallTilemap[_X][_Y] == nullptr)
	{
		LightMap[_X][_Y] = _intensity;
	}
	else
	{
		LightMap[_X][_Y] = 0;
	}
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

	for (int i = 0; i < 500; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			if (SpecialTilemap[j][i] != nullptr)
			{
				CWindowUtilities::Draw(&SpecialTilemap[j][i]->Sprite, SpecialTilemap[j][i]->Sprite.getPosition());
			}
		}
	}


}
