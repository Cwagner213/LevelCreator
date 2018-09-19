//**********************************************************************************************
// MapCreator.h 
// Christopher Wagner  9/14/2018
//
// Header file for MapCreator.cpp which creates a 2D vector grid representing
// a game level with floor and wall tiles.
//**********************************************************************************************

#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include <stdlib.h>    
#include "Vector2.h"
#include "Room.h"


class MapCreator
{
public:
	int startx = 0, starty = 0, endx = 0, endy = 0;
	std::vector<std::vector<char>> getGrid();
	std::vector<std::vector<bool>> getTaken();
	//**********************************************************************************************
	// CreateLevel starts the process of creating a new map on a 2d vector grid.
	//**********************************************************************************************
	void CreateLevel();
	//**********************************************************************************************
	// CreateRoom creates a room that will be returned to be placed inside of the level
	// Returns the room in a two dimensional string array with labels such as
	// null    floor   wall   
	//**********************************************************************************************
	Room CreateRoom();
	//**********************************************************************************************
	// CreateNearRoom takes a room as a parameter and creates a nearby room to return.  
	// This room is first checked to make sure it is a valid spot on the grid
	//**********************************************************************************************
	Room CreateNearRoom(Room roomN);
	//**********************************************************************************************
	// CheckNewZone takes in a room object and information about it and checks to make sure
	// that it will not be placed inside of other rooms.
	//**********************************************************************************************
	bool CheckNewZone(Vector2 Org, int height, int width);
	//**********************************************************************************************
	// CreateConnection creates a 2 direction path between the two rooms given as parameters
	//**********************************************************************************************
	bool CreateConnection(Room room1, Room room2);
	//**********************************************************************************************
	// EasyPath creates a straight two wide passage between the two rooms passed into the function
	//**********************************************************************************************
	bool EasyPath(Room room1, Room room2);
	//**********************************************************************************************
	// Given two rooms it will return the start point for the first room to start corridor creation
	// for a corridor that will lead to the second room;
	//**********************************************************************************************
	Vector2 CreateStart(Room room1, Room room2, int horizontal, int vertical);
	//**********************************************************************************************
	// Room 1 is the room with the start point Room2 is the room being connected to
	// the start point is the start of the corridor in room1
	//**********************************************************************************************
	Vector2 CreateEndPoint(Room room1, Room room2, Vector2 start, int horizontal, int vertical);
	//**********************************************************************************************
	// EncaseGrid searches throught the whole grid and places walls around every floor tile in
	// untaken grid spots fulling encasing the new level 
	//**********************************************************************************************
	void EncaseGrid();
	//**********************************************************************************************
	// Creates a nearby Origin for a new room to be made making sure that
	// it is close enough to make connecting them reasonable
	//**********************************************************************************************
	Vector2 CreateNearOrigin(Room r, Room nR);
	//**********************************************************************************************
	// PrintGrid prints out the grid onto the console in a way that makes it readable
	//**********************************************************************************************
	void PrintGrid();
};