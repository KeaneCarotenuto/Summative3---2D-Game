#include "CPlayer.h"

CPlayer::CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col)
	: Loadable("Player/", "PlayerData")
{
	rect.setPosition(_pos);
	rect.setSize(_size);
	rect.setFillColor(_col);
	Hunger = Data->FileData.GetDataByID("Hunger");
	Health = Data->FileData.GetDataByID("Health");
	if (Health == 0) { Health = 100; }
	Font.loadFromFile("Resources/Fonts/uni.ttf");
	HealthText.setFont(Font);
	HungerText.setFont(Font);
	HealthText.setOrigin(sf::Vector2f(50, -60));
	HungerText.setOrigin(sf::Vector2f(50, -90));
	HealthText.setScale(sf::Vector2f(0.3f, 0.3f));
	HungerText.setScale(sf::Vector2f(0.3f, 0.3f));
	
}

CPlayer::~CPlayer()
{
	GameData SaveData(*this);
	SaveData.Save(FilePath, "PlayerData");
}

void CPlayer::Update(float _fDeltaTime)
{
	//draw text
	HealthText.setPosition(rect.getPosition());
	HungerText.setPosition(rect.getPosition());
	CWindowUtilities::Draw(&rect);
	CWindowUtilities::Draw(&HealthText);
	CWindowUtilities::Draw(&HungerText);

	//Updates death stuff
	if (!dead && Health <= 0) {
		dead = true;
		deathStep = currentStep;
	}

	//Caps values

	if (Health > 100) {
		Health = 100;
	}

	if (Hunger < 0) {
		Hunger = 0;
	}

	if (Hunger > 100) {
		Hunger = 100;
	}
}

void CPlayer::FixedUpdate()
{
	//Movement
	Movement();

	//sets center to player
	CWindowUtilities::ScreenCentre = rect.getPosition();
	
	Hunger += 0.01;
	
	if (Hunger > 50 && Health > 0 )
	{
		Health -= 0.03;
	}

	//Updates text
	HealthText.setString("Health:" + std::to_string((int)Health));
	HungerText.setString("Hunger:" + std::to_string((int)Hunger));
	HealthText.setFillColor((Health < 20 ? sf::Color::Red : sf::Color::White));
	HungerText.setFillColor((Hunger > 50 ? sf::Color::Red : sf::Color::White));

	//Counts steps
	currentStep++;
}

/// <summary>
/// Manages input and movement
/// </summary>
void CPlayer::Movement()
{
	if (currentStep % 1 == 0) {
		//Horizontal
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			
			rect.move(-moveSpeed, 0);
			if (WorldLayer::currentWorld->CheckCollision(rect.getPosition()))
			{
				rect.move(moveSpeed, 0);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			rect.move(moveSpeed, 0);
			if (WorldLayer::currentWorld->CheckCollision(rect.getPosition()))
			{
				rect.move(-moveSpeed, 0);
			}
		}

		//Vertical
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			rect.move(0, -moveSpeed);
			if (WorldLayer::currentWorld->CheckCollision(rect.getPosition()))
			{
				rect.move(0, moveSpeed);
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			rect.move(0, moveSpeed);
			if (WorldLayer::currentWorld->CheckCollision(rect.getPosition()))
			{
				rect.move(0, -moveSpeed);
			}
		}
	}
}


/// <summary>
/// Unused - Was used to walk around map
/// </summary>
void CPlayer::ScreenWrap()
{
	//Horizontal
	if (rect.getPosition().x > 1000 - rect.getGlobalBounds().width) rect.setPosition(0, rect.getPosition().y);
	if (rect.getPosition().x < 0) rect.setPosition(1000 - rect.getGlobalBounds().width, rect.getPosition().y);
	
	//Vertical
	if (rect.getPosition().y > 1000 - rect.getGlobalBounds().height) rect.setPosition(rect.getPosition().x, 0);
	if (rect.getPosition().y < 0) rect.setPosition(rect.getPosition().x, 1000 - rect.getGlobalBounds().height);
}
