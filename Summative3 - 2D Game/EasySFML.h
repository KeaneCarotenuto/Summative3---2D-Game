////////////////////////////////////////////////////////////
//==========================================================
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//----------------------------------------------------------
// (c) 2020 Media Design School
//==========================================================
//   File Name  : EasySFML.h
//----------------------------------------------------------
//  Description : Contains the definition and prototypes
//  for the CGameObject, CObjectController, and
//  CWindowUtilities classes that make up the EasySFML
//  framework.
//
//  EasySFML is a framework intended to streamline SFML
//  development by allowing developers to utilise a code
//  structure more similar to popular game engines.
//
//  It does this by introducing a CGameObject base class
//  that changes the way object instances in the game
//  function by allowing any derived object to independantly
//  run code without relying on their methods being called
//  manually for each instance by the developer.
//
//  Classes derived from CGameObject can choose to implement
//  any number/combination of three methods:
//
//  - Update() Is called every frame, the change in time since the last frame was drawn, called deltaTime, is given as a parameter
//  - FixedUpdate() Is called at a fixed interval that does not vary with framerate
//  - LateUpdate() Is called after every other update method is complete, deltaTime is given as a parameter
//
//  Every implemented method will be called accordingly by
//  the CObjectController class as long as the UpdateObjects()
//  method is called within the game loop.
//
//  CGameObject derived classes also have the bIsEnabled property which is always true on instantiation.
//  This can be useful for when you need to disable instances of objects without destroying them.
//
//  CWindowUtilities is a static class that addresses any issues with accessing the RenderWindow instance from difficult areas in your code
//  it accomplishes this by allowing the developer to add Drawable objects to a vector from anywhere in the code, which is then used to draw
//  each of the objects using the RenderWindow in the main loop.
//
//----------------------------------------------------------
//    Author    : Nerys Thamm BSE20021
//----------------------------------------------------------
//    E-mail    : NerysThamm@gmail.com
//==========================================================
////////////////////////////////////////////////////////////
#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <vector>

// --------------------------------------------------------------------------------
/// <summary>
/// The CGameObject base class allows derived classes to implement the Update() FixedUpdate() and LateUpdate() methods,
/// meaning that these objects can have functionality similar to Scripts in Game Engines such as Unity, Unreal, and Gamemaker
/// </summary>
// --------------------------------------------------------------------------------
class CGameObject
{
	friend class CObjectController;
public:
	// ********************************************************************************
	/// <summary>
	/// This method is called once every frame if it is defined in a derived class
	/// </summary>
	/// <param name="_fDeltaTime">Type: Float, Time since last frame</param>
	// ********************************************************************************
	virtual void Update(float _fDeltaTime) {};

	// ********************************************************************************
	/// <summary>
	/// This method is called at fixed intervals if it is defined in a derived class
	/// </summary>
	// ********************************************************************************
	virtual void FixedUpdate() {};

	// ********************************************************************************
	/// <summary>
	/// This method is called after all other update methods are complete if it is defined in a derived class
	/// </summary>
	/// <param name="_fDeltaTime">Type: Float, Time since last frame</param>
	// ********************************************************************************
	virtual void LateUpdate(float _fDeltaTime) {};

	/// <summary>Stores whether the derived object is enabled</summary>
	bool bIsEnabled = true;
protected:
	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Constructor for Class: CGameObject
	/// Adds the instance to the pool of updatable objects
	/// </para>
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	CGameObject();

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Destructor for Class: CGameObject
	/// Removes the instance from the pool of updatable objects
	/// </para>
	/// </summary>
	/// <returns></returns>
	// ********************************************************************************
	virtual ~CGameObject();
private:
	/// <summary>Pointer to the next CGameObject in the linked list of updatable objects</summary>
	CGameObject* m_pNext;

	/// <summary>Pointer to the previous CGameObject in the linked list of updatable objects</summary>
	CGameObject* m_pPrev;
};
// --------------------------------------------------------------------------------
/// <summary>
/// A static class that controls the functionality of CGameObject derived classes
/// </summary>
// --------------------------------------------------------------------------------
class CObjectController
{
	friend class CGameObject;
public:

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: UpdateObjects
	/// Function is part of Class: CObjectController
	/// Calls the update methods defined by classes inheriting from CGameObject
	/// Must be called within the game loop once every frame.
	/// </para>
	/// </summary>
	// ********************************************************************************
	static void UpdateObjects();

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: Update
	/// Function is part of Class: CObjectController
	/// Calls the Update method on all instances of classes that inherit from CGameObject and define the method
	/// Called once every frame
	/// </para>
	/// </summary>
	/// <param name="_fDeltaTime"><para>
	/// Type: float, time since last frame
	/// </para></param>
	// ********************************************************************************
	static void Update(float _fDeltaTime);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: FixedUpdate
	/// Function is part of Class: CObjectController
	/// Calls the FixedUpdate method on instances of classes that inherit from CGameObject and define the method
	/// Called at fixed intervals independant of framerate
	/// </para>
	/// </summary>
	// ********************************************************************************
	static void FixedUpdate();

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: LateUpdate
	/// Function is part of Class: CObjectController
	/// Calls the LateUpdate method on instances of classes that inherit from CGameObject and define the method
	/// Called once every frame, after all other update methods have finished
	/// </para>
	/// </summary>
	/// <param name="_fDeltaTime"><para>
	/// Type: float, time since last frame
	/// </para></param>
	// ********************************************************************************
	static void LateUpdate(float _fDeltaTime);
private:
	/// <summary>Measures the time between frames</summary>
	static sf::Clock deltaClock;

	/// <summary>The amount of time since the last frame</summary>
	static float deltaTime;

	/// <summary>The fixed interval to call FixedUpdate on</summary>
	static float fixedTime;

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: AddUpdater
	/// Function is part of Class: CObjectController
	/// Adds a CGameObject derived class instance to the pool of instances to call update methods on
	/// </para>
	/// </summary>
	/// <param name="_updateable"><para>
	/// Type: CGameObject Pointer
	/// </para></param>
	// ********************************************************************************
	static void AddUpdater(CGameObject* _updateable);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: RemoveUpdater
	/// Function is part of Class: CObjectController
	/// Removes an instance from the pool of updated objects, called in the destructor
	/// </para>
	/// </summary>
	/// <param name="_updateable"><para>
	/// Type: CGameObject Pointer
	/// </para></param>
	// ********************************************************************************
	static void RemoveUpdater(CGameObject* _updateable);

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: GetHead
	/// Function is part of Class: CObjectController
	/// </para>
	/// </summary>
	/// <returns>Value of type: CGameObject Pointer</returns>
	// ********************************************************************************
	static CGameObject** GetHead();
};
// --------------------------------------------------------------------------------
/// <summary>
/// A static class containing utilities for working with SFML windows
/// </summary>
// --------------------------------------------------------------------------------
class CWindowUtilities
{
public:
	/// <summary>A vector of all the objects to be drawn this frame</summary>
	static std::vector<sf::Drawable*> ToDrawList;
	static std::vector<CGameObject*> ToDeleteList;

	static sf::Vector2f ScreenCentre;
	static float RenderDistance;

	// ********************************************************************************
	/// <summary>
	/// <para>
	/// Function name: Draw
	/// Function is part of Class: CWindowUtilities
	/// Adds an object to the collection of things to be drawn next frame
	/// </para>
	/// </summary>
	/// <param name="Draw"><para>
	/// Type: sf::Drawable Pointer
	/// </para></param>
	// ********************************************************************************
	static void Draw(sf::Drawable* Draw);
	static void Draw(sf::Drawable* Draw, sf::Vector2f Pos);
};
