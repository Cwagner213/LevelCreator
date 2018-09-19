
//*******************************************************************************************************
// APath.h 
// Christopher Wagner  9/5/2018
//
// Header file for Apath.cpp
//*******************************************************************************************************
#pragma once
#include "stdafx.h"
#include <vector>
#include <stdlib.h>    
#include "Node.h"
#include "Grid.h"
#include "Vector2.h"
using namespace std;

class APath
{
public:
	Grid temp;
	vector<Node*> tempPath;
	vector<Node*> ttempPath;
	int tempDistance = 0;
	bool pathIncomplete = false;


	//*******************************************************************************************************
	// Start is a void function that takes a Grid object and sets the Apath object's grid to be equal 
	// with it.  
	//*******************************************************************************************************
	void Start(Grid gridd);
	//*******************************************************************************************************
	// findPath is a function that returns a vector<Node*>, it takes two Vector2 parameters called start and
	// end which represent the starting place and end place for the pathfinding to find a path for
	//*******************************************************************************************************
	vector<Node*> findPath(Vector2 start, Vector2 end);
	//*******************************************************************************************************
	// getDistance takes two Nodes as paraneters to find the distance between them then returns the distance 
	//*******************************************************************************************************
	int getDistance(Node* start, Node* end);
	//*******************************************************************************************************
	// retracePath returns a vector<Node*> containing Node pointers in the order of the path needed to get 
	// from the start Node to the Target Node.  This function takes two parameters, start and target, 
	// representing the start location and end location on the grid.
	//*******************************************************************************************************
	vector<Node*> retracePath(Node* start, Node* target);
	//*******************************************************************************************************
	// containsNode returns a boolean that represents whether or not the paramter check could be found within
	// the vector<Node*> target
	//*******************************************************************************************************
	bool containsNode(Node* check, vector<Node*> target);

	APath()
	{

	}
};