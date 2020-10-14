#include "CPlayer.h"

CPlayer::CPlayer(sf::Vector2f _pos, sf::Vector2f _size, sf::Color _col)
	: Loadable("Player/", "PlayerData")
{
	rect.setPosition(_pos);
	rect.setSize(_size);
	rect.setFillColor(_col);
	Health = Data->FileData.m_Data[0];
	Hunger = Data->FileData.m_Data[1];
	Thirst = Data->FileData.m_Data[2];
	IsBleeding = Data->FileData.m_Data[3];
	Str = Data->FileData.m_Data[4];
	Agi = Data->FileData.m_Data[5];
	Wis = Data->FileData.m_Data[6];
	Def = Data->FileData.m_Data[7];
	Effects = Data->FileData.m_Groups[0];
	TestVector = Data->FileData.m_Groups[1];

}

CPlayer::~CPlayer()
{
	GameData SaveData(*this);
	SaveData.Save(FilePath, "PlayerData");
}

void CPlayer::FixedUpdate()
{
	Movement();

	//ScreenWrap();

	CWindowUtilities::Draw(&rect);
	Hunger += 0.001;
	Thirst += 0.01;
	if (Hunger > 50 || Thirst > 50)
	{
		Health -= 0.5;
	}
	currentStep++;
}

void CPlayer::Movement()
{
	if (currentStep % 5 == 0) {
		//Horizontal
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			rect.move(-moveSpeed, 0);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			rect.move(moveSpeed, 0);
		}

		//Vertical
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			rect.move(0, -moveSpeed);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			rect.move(0, moveSpeed);
		}
	}
}

void CPlayer::ScreenWrap()
{
	//Horizontal
	if (rect.getPosition().x > 1000 - rect.getGlobalBounds().width) rect.setPosition(0, rect.getPosition().y);
	if (rect.getPosition().x < 0) rect.setPosition(1000 - rect.getGlobalBounds().width, rect.getPosition().y);
	
	//Vertical
	if (rect.getPosition().y > 1000 - rect.getGlobalBounds().height) rect.setPosition(rect.getPosition().x, 0);
	if (rect.getPosition().y < 0) rect.setPosition(rect.getPosition().x, 1000 - rect.getGlobalBounds().height);
}
