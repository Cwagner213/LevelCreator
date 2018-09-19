//*******************************************************************************************************
// Node.h
// Christopher Wagner  9/5/2018
//
// Standard linked list Node which has variables used in the pathfinding algorithm
//*******************************************************************************************************
#pragma once
#include "stdafx.h"
#include <stdlib.h>   
#include "Vector2.h"

class Node
{
public:
	bool walkable;
	Vector2 position;
	int counter;
	int DistanceFrom;
	int MoveCost;
	int TotalScore();
	Node* getParent();
	Node *parent;
	bool pUsed;

	Node()
	{

	}

	Node(bool walk, Vector2 pos)
	{
		walkable = walk;
		position = pos;
	}
	void setParent(Node* p);
};