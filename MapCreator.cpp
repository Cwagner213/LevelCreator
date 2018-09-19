//**********************************************************************************************
// MapCreator.cpp
// Christopher Wagner  9/14/2018
//
// CPP file for MapCreator.cpp which creates a 2D vector grid representing
// a game level with floor and wall tiles.
//**********************************************************************************************

#pragma once
#include "stdafx.h"
#include <iostream>
#include <ctime>
#include "MapCreator.h"
#include <random>
#include "Vector2.h"

using namespace std;

int startx = 0, starty = 0, endx = 0, endy = 0;
int LevelSizeY = 150;
int LevelSizeX = 150;
time_t t;
int RoomAmount = 6;
int maxRoomHeight;
int maxRoomWidth;
vector<vector<char>> grid(150, vector<char>(150));
vector<vector<bool>> taken(150, vector<bool>(150));


vector<vector<char>> MapCreator::getGrid()
{
	return grid;
}

vector<vector<bool>> MapCreator::getTaken()
{
	return taken;
}

void MapCreator::CreateLevel()
{
	srand(time(&t));

	Room r = CreateRoom(); //Create original room
	startx = r.origin.x + 2; //Set room origin
	starty = r.origin.y + 2;

	vector<Room> Roomlist = { r }; ; //create vector of rooms with the original room in the first slot
	 

	//Sets the grid and taken grid to empty with no floor or walls
	for (int i = 0; i < LevelSizeY; i++)
	{
		for (int q = 0; q < LevelSizeX; q++)
		{
			grid[i][q] = 'n';
			taken[i][q] = false;
		}
	}

	//Places the first room onto the grid setting the tiles to 'f' for floor and true for moveable
	for (int i = r.origin.y; i < r.origin.y + r.height; i++)
	{
		for (int q = r.origin.x; q < r.origin.x + r.width; q++)
		{
			grid[q][i] = 'f';
			taken[i][q] = true;
		}
	}

	//Constructs the rest of the level beyond the first floor
	for (int i = 1; i < RoomAmount; i++)
	{
		Room ro = CreateNearRoom(Roomlist[i - 1]); //Create new room near the last room made 
		Roomlist.push_back(ro); //add the new room to the roomlist

		//Sets the end target for the pathfinding to test with if its the last room made
		if (i == RoomAmount - 1)
		{
			endx = ro.origin.x + 2;
			endy = ro.origin.y + 2;
		}

		//Set the new room into the corresponding grids
		for (int q = ro.origin.x; q < ro.origin.x - 1 + ro.width; q++)
		{
			for (int p = (int)ro.origin.y; p < ro.origin.y - 1 + ro.height; p++)
			{
				taken[q][p] = true;
				grid[q][p] = 'f';
			}
		}

		//Attempts to make an easy straight path between two rooms, if it fails then it makes a two directional path
		EasyPath(Roomlist[i], Roomlist[i - 1]);
		if (EasyPath(Roomlist[i], Roomlist[i - 1]) == false)
		{
			CreateConnection(Roomlist[i], Roomlist[i - 1]); //Two directional path from room to room
		}



		//Randomly creates extra rooms to randomize level size
		if (rand() % 1 == 0)
		{
			//Creates new room near the last one
			ro = CreateNearRoom(Roomlist[i - 1]);
			Roomlist[i] = ro;

			//Sets new rooms grid 
			for (int q = ro.origin.x; q < ro.origin.x - 1 + ro.width; q++)
			{
				for (int p = (int)ro.origin.y; p < ro.origin.y - 1 + ro.height; p++)
				{
					taken[q][p] = true;
					grid[q][p] = 'f';

				}
			}
			EasyPath(Roomlist[i], Roomlist[i - 1]);
			if (EasyPath(Roomlist[i], Roomlist[i - 1]) == false)
			{
				CreateConnection(Roomlist[i], Roomlist[i - 1]);
			}
		}
	}
	//Adds random connections between rooms to give the levels a less linear feel
	for (int i = 0; i < RoomAmount / 5; i++)
	{
		if (EasyPath(Roomlist[rand() % RoomAmount], Roomlist[rand() % RoomAmount]) == false)
		{
			CreateConnection(Roomlist[rand() % RoomAmount], Roomlist[rand() % RoomAmount]);
		}
	}

	EncaseGrid();
}
//**********************************************************************************************
// CreateRoom creates a room that will be returned to be placed inside of the level
// Returns the room in a two dimensional string array with labels such as
// null    floor   wall   
//**********************************************************************************************
Room MapCreator::CreateRoom()
{
	//Makes a level sized grid to fit where the new room be placed
	Room room;

	//Loop to generate a random room size and location then checks to make sure it doesnt
	//overlap with other rooms or corridors
	int x = 0;
	do
	{
		room.width = rand() % 7 + 4;
		room.height = rand() % 7 + 4;
		room.origin.x = 75;
		room.origin.y = 75;
		x++;
		if (x == 200)
			break;
	} while (CheckNewZone(room.origin, room.height, room.width) == false);

	//returns the new room to be added into the level
	return room;
}





//**********************************************************************************************
// CreateNearRoom takes a room as a parameter and creates a nearby room to return.  
// This room is first checked to make sure it is a valid spot on the grid
//**********************************************************************************************
Room MapCreator::CreateNearRoom(Room roomN)
{
	//Makes a level sized grid to fit where the new room be placed
	Room room;

	//Loop to generate a random room size and location then checks to make sure it doesnt
	//overlap with other rooms or corridors
	int x = 0;
	bool Con = false;
	while (Con == false)
	{
		x++;
		room.width = rand() % 7 + 4;
		room.height = rand() % 7 + 4;

		room.origin = CreateNearOrigin(roomN, room);

		if (CheckNewZone(room.origin, room.height, room.width) == true)
		{
			Con = true;
		}
		if (x == 60)
			break;
	}
	//returns the new room to be added into the level
	return room;
}


//**********************************************************************************************
// CheckNewZone takes in a room object and information about it and checks to make sure
// that it will not be placed inside of other rooms.
//**********************************************************************************************
bool MapCreator::CheckNewZone(Vector2 Org, int height, int width)
{

	//Makes sure the room doesnt leave the level in an vertical direction
	if (Org.y + height >= LevelSizeY - 1)
		return false;

	//Makes sure the room doesnt leave the level in a horizontal direction
	if (Org.x + width >= LevelSizeX - 1)
		return false;

	if (Org.x < 3 || Org.y < 3)
		return false;
	//Checks to make sure the room isnt being placed inside of another room
	//or corridor
	for (int i = Org.x; i < Org.x + width; i++)
	{
		for (int q = Org.y; q < Org.y + height; q++)
		{
			if (taken[i][q] == true)
			{
				return false;
			}
		}
	}

	//returns true if the room placement is valid
	return true;
}
//**********************************************************************************************
//  CreateConnection creates a 2 direction path between the two rooms given as parameters
//**********************************************************************************************
bool MapCreator::CreateConnection(Room room1, Room room2)
{

	int horizontal, vertical;


	//Sets horizontal and vertical to negative or positive integers equal to the difference
	//of the two rooms origin points in order to find out which direction the rooms are in respect
	//to eachother
	if (room1.origin.x > room2.origin.x)
	{
		horizontal = -(room1.origin.x - room2.origin.x);
	}
	else if (room1.origin.x < room2.origin.x)
	{
		horizontal = (room2.origin.x - room1.origin.x);
	}
	else
	{
		horizontal = 0;
	}
	if (room1.origin.y > room2.origin.y)
	{
		vertical = -(room1.origin.y - room2.origin.y);
	}
	else if (room1.origin.y < room2.origin.y)
	{
		vertical = (room2.origin.y - room1.origin.y);
	}
	else
	{
		vertical = 0;
	}

	Vector2 Start = CreateStart(room1, room2, horizontal, vertical);
	Vector2 End = CreateEndPoint(room1, room2, Start, horizontal, vertical);


	//adds corridor onto the grid
	if (Start.y == room1.origin.y + room1.height)
	{
		//Adds corridors Y movement to the grid

		for (int i = -1; i < (End.y - Start.y) + 12; i++)
		{
			if (taken[Start.x][Start.y + i] == false)
			{
				if (i == -1)
				{
					grid[Start.x][Start.y + i] = 'f';
					taken[Start.x][Start.y + i] = true;
				}
				else
				{
					taken[Start.x][Start.y + i] = true;
					grid[Start.x][Start.y + i] = 'f';
				}
			}
		}
		//Adds corridors X movement to the grid if the second room is to the right of the first one
		if (horizontal > 0)
		{
			for (int i = 0; i < End.x - Start.x + 2; i++)
			{
				taken[Start.x + i][Start.y + (End.y - Start.y)] = true;
				grid[Start.x + i][Start.y + (End.y - Start.y)] = 'f';
			}
			return true;
		}
		//Adds corridors X movement to the grid if the second room is to the left of the first one
		else
		{
			for (int i = 0; i < Start.x - End.x + 2; i++)
			{
				taken[Start.x - i][Start.y + (End.y - Start.y)] = true;
				grid[Start.x - i][Start.y + (End.y - Start.y)] = 'f';
			}
			return true;
		}
	}

	else if (Start.y == room1.origin.y)
	{
		//Adds corridors Y movement to the grid
		for (int i = -1; i < (Start.y - End.y) + 12; i++)
		{
			if (taken[Start.x][Start.y - i] == false)
			{
				taken[Start.x][Start.y - i] = true;
				grid[Start.x][Start.y - i] = 'f';
			}
		}
		//Adds corridors X movement to the grid if the second room is to the right of the first one
		if (horizontal > 0)
		{
			for (int i = 0; i < End.x - Start.x + 2; i++)
			{
				taken[Start.x + i][Start.y - (Start.y - End.y)] = true;
				grid[Start.x + i][Start.y - (Start.y - End.y)] = 'f';
			}
			return true;
		}
		//Adds corridors X movement to the grid if the second room is to the left of the first one
		else
		{
			for (int i = 0; i < Start.x - End.x + 2; i++)
			{
				taken[Start.x - i][Start.y - (Start.y - End.y)] = true;
				grid[Start.x - i][Start.y - (Start.y - End.y)] = 'f';
			}
			return true;
		}
	}

	else if (Start.x == room1.origin.x)
	{
		//Adds the corridor X movement to the grid
		for (int i = -1; i < (Start.x - End.x) + 12; i++)
		{
			if (taken[Start.x - i][Start.y] == false)
			{
				taken[Start.x - i][Start.y] = true;
				grid[Start.x - i][Start.y] = 'f';
			}
		}
		//Adds the corridor Y movement if the Y movement is upward
		if (vertical > 0)
		{
			for (int i = 0; i < End.y - Start.y + 2; i++)
			{
				taken[Start.x + (End.x - Start.x)][Start.y + i] = true;
				grid[Start.x + (End.x - Start.x)][Start.y + i] = 'f';
			}
			return true;
		}
		//Adds the corridor Y movement if the Y movement is downward
		else
		{
			for (int i = 0; i < Start.y - End.y + 2; i++)
			{
				taken[Start.x + (End.x - Start.x)][Start.y - i] = true;
				grid[Start.x + (End.x - Start.x)][Start.y - i] = 'f';
			}
			return true;
		}
	}
	else if (Start.x == room1.origin.x + room1.width)
	{
		for (int i = -1; i < (End.x - Start.x) + 12; i++)
		{
			if (taken[Start.x + i][Start.y] == false)
			{
				taken[Start.x + i][Start.y] = true;
				grid[Start.x + i][Start.y] = 'f';
			}
		}
		if (vertical > 0)
		{
			for (int i = 0; i < End.y - Start.y + 2; i++)
			{
				taken[Start.x + (End.x - Start.x)][Start.y + i] = true;
				grid[Start.x + (End.x - Start.x)][Start.y + i] = 'f';
			}
			return true;
		}
		else
		{
			for (int i = 0; i < Start.y - End.y + 2; i++)
			{
				taken[Start.x + (End.x - Start.x)][Start.y - i] = true;
				grid[Start.x + (End.x - Start.x)][Start.y - i] = 'f';
			}
			return true;
		}
	}

	return false;
}


//**********************************************************************************************
//  EasyPath creates a straight two wide passage between the two rooms passed into the function
//**********************************************************************************************
bool MapCreator::EasyPath(Room room1, Room room2)
{
	int horizontal, vertical;


	//Sets horizontal and vertical to negative or positive integers equal to the difference
	//of the two rooms origin points in order to find out which direction the rooms are in respect
	//to eachother
	if (room1.origin.x > room2.origin.x)
	{
		horizontal = -((int)room1.origin.x - (int)room2.origin.x);
	}
	else if (room1.origin.x < room2.origin.x)
	{
		horizontal = ((int)room2.origin.x - (int)room1.origin.x);
	}
	else
	{
		horizontal = 0;
	}
	if (room1.origin.y > room2.origin.y)
	{
		vertical = -((int)room1.origin.y - (int)room2.origin.y);
	}
	else if (room1.origin.y < room2.origin.y)
	{
		vertical = ((int)room2.origin.y - (int)room1.origin.y);
	}
	else
	{
		vertical = 0;
	}

	Vector2 s1;
	Vector2 s2;
	Vector2 e1;
	Vector2 e2;

	int countx = 0;
	int county = 0;

	for (int i = 0; i < room1.width; i++)
	{
		//Checks to see if there is a valid connection vertically between the two rooms
		if (room1.origin.x + i >= room2.origin.x && room1.origin.x + i <= room2.origin.x + room2.width)
		{
			countx++;
			//If count reaches 2 then there is enough room for a straight 2 wide path between the two rooms
			if (countx == 2)
			{
				//Checks to see which room in on top of the other and sets the start and end vectors accordingly
				if (vertical > 0)
				{
					s1.x = room1.origin.x + i;
					s1.y = room1.origin.y + room1.height;

					s2.x = room1.origin.x + i - 1;
					s2.y = room1.origin.y + room1.height;

					e1.x = room1.origin.x + i;
					e1.y = room2.origin.y;

					e2.x = room1.origin.x + i - 1;
					e2.y = room2.origin.y;
				}
				else
				{
					s1.x = room1.origin.x + i;
					s1.y = room1.origin.y;

					s2.x = room1.origin.x + i - 1;
					s2.y = room1.origin.y;

					e1.x = room1.origin.x + i;
					e1.y = room2.origin.y + room2.height;

					e2.x = room1.origin.x + i - 1;
					e2.y = room2.origin.y + room2.height;
				}

				//Adds the corridor to the grid
				//If s1 is larger than e1 then the corridor is heading downwards
				if (s1.y > e1.y)
				{
					for (int q = -1; q < s1.y - e1.y + 3; q++)
					{
						if (taken[s1.x][s1.y - q] == false)
						{
							grid[s1.x][s1.y - q] = 'f';
							taken[s1.x][s1.y - q] = true;
						}
						if (taken[s2.x][s2.y - q] == false)
						{
							grid[s2.x][s2.y - q] = 'f';
							taken[s2.x][s2.y - q] = true;
						}
					}
					return true;
				}
				//if s1 is not larger than e1 then the corridor is heading upwards
				else
				{
					for (int q = -1; q < e1.y - s1.y + 3; q++)
					{
						if (taken[s1.x][s1.y + q] == false)
						{
							grid[s1.x][s1.y + q] = 'f';
							taken[s1.x][s1.y + q] = true;
						}
						if (taken[s2.x][s2.y + q] == false)
						{
							grid[s2.x][s2.y + q] = 'f';
							taken[s2.x][s2.y + q] = true;
						}
					}
					return true;
				}

			}
		}
	}
	for (int i = 0; i < room1.height; i++)
	{
		//Checks for valid horizontal paths between the two rooms
		if (room1.origin.y + i >= room2.origin.y && room1.origin.y + i <= room2.origin.y + room2.height)
		{
			//if count reaches 2 then a valid path has been found
			county++;
			if (county == 2)
			{
				//Checks to see if the room is to the right or to the left of the initial room
				if (horizontal > 0)
				{
					s1.x = room1.origin.x + room1.width;
					s1.y = room1.origin.y + i - 1;

					s2.x = room1.origin.x + room1.width;
					s2.y = room1.origin.y + i;

					e1.x = room2.origin.x;
					e1.y = room1.origin.y + i - 1;

					e2.x = room2.origin.x;
					e2.y = room1.origin.y + i;
				}
				else
				{
					s1.x = room1.origin.x;
					s1.y = room1.origin.y + i - 1;

					s2.x = room1.origin.x;
					s2.y = room1.origin.y + i;

					e1.x = room2.origin.x + room2.width;
					e1.y = room1.origin.y + i - 1;

					e2.x = room2.origin.x + room2.width;
					e2.y = room1.origin.y + i;
				}

				//adds corridor to grid 
				//if s1 is greater than e1 then the corridor is heading to the left
				if (s1.x > e1.x)
				{
					for (int q = -1; q < s1.x - e1.x + 3; q++)
					{
						if (taken[s1.x - q][s1.y] == false)
						{
							grid[s1.x - q][s1.y] = 'f';
							taken[s1.x - q][s1.y] = true;
						}
						if (taken[s2.x - q][s2.y] == false)
						{
							grid[s2.x - q][s2.y] = 'f';
							taken[s2.x - q][s2.y] = true;
						}
					}
					return true;
				}
				//if s1 is less than e1 then the corridor is heading to the right
				else
				{
					for (int q = -1; q < e1.x - s1.x + 3; q++)
					{
						if (taken[s1.x + q][s1.y] == false)
						{
							grid[s1.x + q][s1.y] = 'f';
							taken[s1.x + q][s1.y] = true;
						}
						if (taken[s2.x + q][s2.y] == false)
						{
							grid[s2.x + q][s2.y] = 'f';
							taken[s2.x + q][s2.y] = true;
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}


//**********************************************************************************************
// Given two rooms it will return the start point for the first room to start corridor creation
// for a corridor that will lead to the second room;
//**********************************************************************************************
Vector2 MapCreator::CreateStart(Room room1, Room room2, int horizontal, int vertical)
{

	Vector2 Start;


	//Finds the starting point for the corridor in the first room
	if (horizontal > 0)
	{
		if (vertical > 0)
		{
			//Sets the start point of the corridor assuming that the second room is up and to the right 
			//of the initial room

			if (rand() % 2 > 0)
			{
				Start.x = (room1.origin.x + 1) + rand() % (room1.width - 1);
				Start.y = room1.origin.y + room1.height;
			}
			else
			{
				Start.x = room1.origin.x + room1.width;
				Start.y = (room1.origin.y + 1) + rand() % (room1.height - 1);
			}

		}
		else if (vertical < 0)
		{
			//Sets the start point of the corridor assuming that the second room is down and to the right
			//of the initial room
			if (rand() % 2 > 0)
			{
				Start.x = (room1.origin.x + 1) + rand() % (room1.width - 1);
				Start.y = room1.origin.y;
			}
			else
			{
				Start.x = room1.origin.x + room1.width;
				Start.y = (room1.origin.y + 1) + rand() % (room1.height - 1);
			}
		}

	}
	else if (horizontal < 0)
	{
		if (vertical > 0)
		{
			//Sets the start point of the corridor assuming that the second room is up and to the left
			//of the initial room
			if (rand() % 2 > 0)
			{
				Start.x = (room1.origin.x + 1) + rand() % (room1.width - 1);
				Start.y = room1.origin.y + room1.height;
			}
			else
			{
				Start.x = room1.origin.x;
				Start.y = (room1.origin.y + 1) + rand() % (room1.height - 1);
			}
		}
		else if (vertical < 0)
		{
			//Sets the start point of the corridor assuming that the second room is down and to the left 
			// of the initial room
			if (rand() % 2 > 0)
			{
				Start.x = room1.origin.x;
				Start.y = (room1.origin.y + 1) + rand() % (room1.height - 1);
			}
			else
			{
				Start.x = (room1.origin.x + 1) + rand() % (room1.width - 1);
				Start.y = room1.origin.y;
			}
		}
	}
	return Start;
}

//**********************************************************************************************
// Room 1 is the room with the start point Room2 is the room being connected to
// the start point is the start of the corridor in room1
//**********************************************************************************************
Vector2 MapCreator::CreateEndPoint(Room room1, Room room2, Vector2 start, int horizontal, int vertical)
{
	Vector2 end;
	if (start.x == room1.origin.x)
	{
		if (vertical > 0)
		{
			end.x = room2.origin.x + rand() % 2;
			end.y = room2.origin.y;

		}
		else if (vertical < 0)
		{
			end.x = room2.origin.x + rand() % 2;
			end.y = room2.origin.y + room2.height;
		}
	}
	else if (start.x == room1.origin.x + room1.width)
	{
		if (vertical > 0)
		{
			end.x = room2.origin.x + rand() % 2;
			end.y = room2.origin.y;

		}
		else if (vertical < 0)
		{
			end.x = room2.origin.x + rand() % 2;
			end.y = room2.origin.y + room2.height;
		}
	}
	else if (start.y == room1.origin.y)
	{
		if (horizontal > 0)
		{
			end.x = room2.origin.x;
			end.y = room2.origin.y + rand() % 2;
		}
		else if (horizontal < 0)
		{
			end.x = room2.origin.x + room2.width;
			end.y = room2.origin.y + rand() % 2;
		}
	}
	else if (start.y == room1.origin.y + room1.height)
	{
		if (horizontal > 0)
		{
			end.x = room2.origin.x;
			end.y = room2.origin.y + rand() % 2;
		}
		else if (horizontal < 0)
		{
			end.x = room2.origin.x + room2.width;
			end.y = room2.origin.y + rand() % 2;
		}
	}

	return end;
}


//**********************************************************************************************
// EncaseGrid searches throught the whole grid and places walls around every floor tile in
// untaken grid spots fulling encasing the new level 
//**********************************************************************************************
void MapCreator::EncaseGrid()
{
	//Checks every element in the grid
	for (int i = 1; i < LevelSizeX; i++)
	{
		for (int q = 1; q < LevelSizeY; q++)
		{
			//if grid at this position is a floor tile
			if (grid[i][q] == 'f')
			{
				//If null spots are found around the floor set them to be walls
				for (int p = 0; p < 8; p++)
				{
					if (p == 0)
					{
						if (grid[i][q + 1] == 'n')
						{
							grid[i][q + 1] = 'w';
						}
					}
					if (p == 1)
					{
						if (grid[i + 1][q + 1] == 'n')
						{
							grid[i + 1][q + 1] = 'w';
						}
					}
					if (p == 2)
					{
						if (grid[i + 1][q] == 'n')
						{
							grid[i + 1][q] = 'w';
						}
					}
					if (p == 3)
					{
						if (grid[i + 1][q - 1] == 'n')
						{
							grid[i + 1][q - 1] = 'w';
						}
					}
					if (p == 4)
					{
						if (grid[i][q - 1] == 'n')
						{
							grid[i][q - 1] = 'w';
						}
					}
					if (p == 5)
					{
						if (grid[i - 1][q - 1] == 'n')
						{
							grid[i - 1][q - 1] = 'w';
						}
					}
					if (p == 6)
					{
						if (grid[i - 1][q] == 'n')
						{
							grid[i - 1][q] = 'w';
						}
					}
					if (p == 7)
					{
						if (grid[i - 1][q + 1] == 'n')
						{
							grid[i - 1][q + 1] = 'w';
						}
					}
				}
			}
		}
	}
}


//**********************************************************************************************
// Creates a nearby Origin for a new room to be made making sure that
// it is close enough to make connecting them reasonable
//**********************************************************************************************
Vector2 MapCreator::CreateNearOrigin(Room r, Room nR)
{
	Vector2 Origin;

	if (rand() % 2 > 0)
	{
		Origin.x = r.origin.x + ((rand() % 2) + 2);
	}
	else
	{
		Origin.x = r.origin.x - (r.width + ((rand() % 2) + 2));
	}
	if (rand() % 2 > 0)
	{
		Origin.y = r.origin.y + ((rand() % 2) + 2);
	}
	else
	{
		Origin.y = r.origin.y - (r.height + ((rand() % 2) + 2));
	}

	return Origin;
}


//**********************************************************************************************
// PrintGrid prints out the grid onto the console in a way that makes it readable
//**********************************************************************************************
void MapCreator::PrintGrid()
{
	for (int i = 0; i < 150; i++)
	{
		for (int j = 1; j < 150; j++)
		{
			if (grid[j][i] == 'n')
			{
				std::cout << ' ';
			}
			else if (grid[j][i] == 'f')
			{
				std::cout << 'x';
			}
			else if (grid[j][i] == 'w')
			{
				std::cout << 'o';
			}
		}
		std::cout << std::endl;
	}
}


