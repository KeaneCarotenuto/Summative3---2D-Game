////////////////////////////////////////////////////////////
//========================================================//
// Bachelor of Software Engineering                       //
// Media Design School                                    //
// Auckland                                               //
// New Zealand                                            //
//--------------------------------------------------------//
// (c) 2020 Media Design School                           //
//========================================================//
//   File Name  :	Globals
//					Stores some global vars
//--------------------------------------------------------//
//					BSE20021
//    Author    :	Nerys Thamm
//					Keane Carotenuto
//--------------------------------------------------------//
//    E-mails   :	NerysThamm@gmail.com
//					Keane Carotenuto@gmail.com
//========================================================//
////////////////////////////////////////////////////////////

#pragma once
#include <SFML/Graphics.hpp>

class Globals
{
public:

	static std::map <std::string, sf::RenderWindow*> mapOfWindows;
	static std::vector<sf::RenderWindow*> inventories;
	static int seed;

	static void RegisterWindow(std::string _str, sf::RenderWindow* _wind);
};