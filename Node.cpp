//*******************************************************************************************************
// Node.cpp 
// Christopher Wagner  9/5/2018
//
// Standard linked list Node which has variables used in the pathfinding algorithm
//*******************************************************************************************************
#pragma once
#include "stdafx.h"
#include <stdlib.h>    
#include "Vector2.h"
#include "Node.h"
#include <iostream>

using namespace std;
	 bool walkable; //Is the Node able to be walked on
	 Vector2 position; //Position of Node on the grid
	 int counter;
	 int DistanceFrom; //Distance from target
	 int MoveCost; //Cost of moving one square on grid, mostly useless in this implementation but for diagnol movement would be useful later
	 bool pUsed = false;
	 Node *parent;

	 int Node::TotalScore()
	 {	
		 return DistanceFrom + MoveCost; 
	 }
	
	 Node* Node::getParent()
	 {
		 return parent;
	 }

	 void Node::setParent(Node* P)
	 {
		 pUsed = true;
		 parent = P;
	 }

