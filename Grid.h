//*******************************************************************************************************
// Grid.h
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

using namespace std;

class Grid
{
public:
	Vector2 worldSize;

	int x, y;
	//*******************************************************************************************************
	// returnGrid returns the current grid of nodes
	//*******************************************************************************************************
	vector<vector<Node*>> returnGrid();
	//*******************************************************************************************************
	//  updateGrid takes an outside vector<vector<bool>> to be translated into Nodes for use in the
	//  pathfinding algorithm 
	//*******************************************************************************************************
	void updateGrid(vector<vector<bool>> gridMove);
	//*******************************************************************************************************
	// GetNeighbors takes a Node pointer to a Node within gridx and returns a vector of Nodes that surround
	// the given Node on the 2D grid
	//*******************************************************************************************************
	vector<Node*> GetNeighbors(Node* P);
};
