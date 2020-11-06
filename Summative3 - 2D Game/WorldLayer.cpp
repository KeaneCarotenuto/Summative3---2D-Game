#include "WorldLayer.h"

WorldLayer* WorldLayer::currentWorld;

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

WorldLayer::~WorldLayer()
{
	for (int y = 0; y < height; y++) //Free up all memory used
	{
		for (int x = 0; x < width; x++)
		{
			delete TerrainTilemap[x][y];
			TerrainTilemap[x][y] = nullptr;

			delete SpecialTilemap[x][y];
			SpecialTilemap[x][y] = nullptr;

			delete WallTilemap[x][y];
			WallTilemap[x][y] = nullptr;
		}
	}
}

WorldLayer::WorldLayer(int _seed)
{
	seed = _seed;
	Altitude = 1;
	//Todo: Generate TileMaps
	populateTileMaps();
	//Todo: Spawn entities
}

WorldLayer::WorldLayer(int _seed, WorldLayer*&_Above, WorldLayer*&_Below, int _altitude)
{
	seed = _seed;
	Altitude = _altitude;
	//Todo: Generate TileMaps
	populateTileMaps();
	//Todo: Spawn entities
}



void WorldLayer::populateTileMaps()
{
	int map[width][height];
	noise::module::Perlin Noise;
	Noise.SetSeed(seed);
	double val;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			
			val = Noise.GetValue((double)(i)*(2.0f/ width), (double)(j) * (2.0f / height), 0.5); //Generate noise
			val -= 0.006 * std::abs(std::sqrt((i - 250) * (i - 250) + (j - 250) * (j - 250))); //Lower value of edges of map for island effect

			/// <summary>
			/// Set terrain type based on height
			/// </summary>
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
	
	
	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			TerrainTilemap[i][j] = new TerrainTile((TerrainType)(map[i][j]), i, j); //Generate tile based on provided type from perlin noise
			
			SpecialTilemap[i][j] = nullptr;
			if (map[i][j] == 6)
			{
				WallTilemap[i][j] = new WallTile(WallType::ROCK, i, j); //Generate rock walls in ultra high terrain 
			}
			else
			{
				WallTilemap[i][j] = nullptr;
			}

			if (map[i][j] == 4 && rand()%3 == 1 )
			{

			}
			else if (map[i][j] == 5 && rand() % 50 == 0)  //Generate boulders and trees to populate the map
			{
				SpecialTilemap[i][j] = new Boulder(sf::Vector2f(i * 20, j * 20));
			}
			else if ( map[i][j] == 3 && rand() % 50 == 0)
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

	float xIndex = floorf(CWindowUtilities::ScreenCentre.x / 20); //Get centre tile
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
	if (xIndexMax >= width)
	{
		xIndexMax = width - 1;
	}
	if (yIndexMax >= height)
	{
		yIndexMax = height - 1;
	}

	m_TerrainTexture.loadFromFile("Resources/Test/TerrainTileset.png");
	m_TerrainVertices.setPrimitiveType(sf::Quads);
	m_TerrainVertices.resize(width * height * 4);
	m_WallTexture.loadFromFile("Resources/Test/WallTileset.png");
	m_WallVertices.setPrimitiveType(sf::Quads);
	m_WallVertices.resize(width * height * 4);


	for (int i = xIndexMin; i < xIndexMax; ++i)
	{
		for (int j = yIndexMin; j < yIndexMax; ++j) //Generated textured Quads
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

sf::Vector2f WorldLayer::GetFirstSandTilePos() //Get coords of first sand tile found so the player doesnt spawn in water
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (TerrainTilemap[i][j]->Type == TerrainType::SAND)
			{
				return sf::Vector2f(i * 20, j * 20);
			}
		}
	}
	return sf::Vector2f(250, 250);
}

/// <summary>
/// CURRENTLY DISABLED
/// </summary>
bool WorldLayer::CheckCollision(sf::Vector2f _nextPos)
{
	//REMOVE IF WANTING COLLISION (MAY BREAK)
	return false;


	int x = floor(_nextPos.x / 20);
	int y = floor(_nextPos.y / 20);

	if (x > 0 && y > 0 && x < width && y < height) {
		return ((SpecialTilemap[x][y] != nullptr) || (WallTilemap[x][y] != nullptr));
	}
	else {
		return false;
	}

	
}

void WorldLayer::resetLightMap()  //Resets the light map
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
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
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

void WorldLayer::renderLightMap() //Renders the light map (WARNING: HIGH RESOURCE USAGE)
{


	/*DEPRECATED: DO NOT USE*/
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
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
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
					if (i < width-1)
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
					if (i < width-1 && j < height-1)
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
					if (j < height-1)
					{

						if (LightMap[i][j + 1] < x)
						{
							LightMap[i][j + 1] = x - 1;
						}

					}
					if (i > 0 && j < height-1)
					{
						if (LightMap[i - 1][j + 1] < x)
						{
							LightMap[i - 1][j + 1] = x - 1;
						}
					}
					if (i < width-1 && j > 0)
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
	m_LightLevelVertices.resize(width * height * 4);

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
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

void WorldLayer::addPointLight(int _X, int _Y, int _intensity) //DISABLED (WARNING: HIGH RESOURCE USAGE DO NOT USE LIGHTING SYSTEM)
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

void WorldLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const //Draw all Quads
{

	

	states.transform *= getTransform();

	states.texture = &m_TerrainTexture;
	target.draw(m_TerrainVertices, states);

	states.texture = &m_WallTexture;
	target.draw(m_WallVertices, states);

	states.texture = &m_LightLevelTexture;
	target.draw(m_LightLevelVertices, states);


}

void WorldLayer::DrawSpecial() //Draw Rocks and trees
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (SpecialTilemap[j][i] != nullptr)
			{
				CWindowUtilities::Draw(&SpecialTilemap[j][i]->Sprite, SpecialTilemap[j][i]->Sprite.getPosition());
			}
		}
	}
}
