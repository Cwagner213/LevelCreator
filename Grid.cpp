//*******************************************************************************************************
// Grid.cpp 
// Christopher Wagner  9/5/2018
//
// Contains methods to assist in pathfinding by recording the walkable and non passable slots of the
// grid/map.
//*******************************************************************************************************
#pragma once
#include "stdafx.h"
#include <vector>
#include <stdlib.h>    
#include "Node.h"
#include "Vector2.h"
#include "Grid.h"

using namespace std;


	vector<vector<Node*>> gridx(150, vector<Node*>(150)); //Node grid representing randomly generated map

	
	void Grid::updateGrid(vector<vector<bool>> gridMove)
	{
		//Sets the grids information to an outside grid
		for (int i = 0; i < 150; i++)
		{
			for (int q = 0; q < 150; q++)
			{
				gridx[i][q] = new Node(gridMove[i][q], Vector2(i, q)); 
			}
		}

	}


	vector<vector<Node*>> Grid::returnGrid()
	{
		return gridx;
	}

	
	vector<Node*> Grid::GetNeighbors(Node* P)
	{
		vector<Node*> Neighbors;
		int x = (int)P->position.x;
		int y = (int)P->position.y;

		//Adds all Nodes to the return vector which are walkable 
		if (gridx[x + 1][y]->walkable == true)
		{
			Neighbors.push_back(gridx[x + 1][y]);
		}
		if (gridx[x - 1][y]->walkable == true)
		{
			Neighbors.push_back(gridx[x - 1][y]);
		}
		if (gridx[x][y + 1]->walkable == true)
		{
			Neighbors.push_back(gridx[x][y + 1]);
		}
		if (gridx[x][y - 1]->walkable == true)
		{
			Neighbors.push_back(gridx[x][y - 1]);
		}
		return Neighbors;
	}


