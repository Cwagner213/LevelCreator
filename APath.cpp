//*******************************************************************************************************
// APath.cpp 
// Christopher Wagner  9/5/2018
//
// Contains methods to assist in pathfinding given a vector<vector<>> of nodes with walkable and 
// parent variables.
//*******************************************************************************************************
#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <stdlib.h>    
#include "Node.h"
#include "Vector2.h"
#include "Grid.h"
#include <algorithm>
#include "APath.h"

using namespace std;

	Grid tempGrid;
	int tempDistance = 0;
	bool pathIncomplete = false;


	void APath::Start(Grid gridd)
	{
		tempGrid = gridd; //Set the classes internal grid 
	}

	vector<Node*> APath::findPath(Vector2 start, Vector2 end)
	{
		vector<Node*> open; //vector to record open nodes to still be checked
		vector<Node*> closed; //vector to record closed nodes that have already been checked

		Node* startNode = new Node; //Pointer to the starting node 
		startNode->position.x = start.x;
		startNode->position.y = start.y;
		Node* targetNode = new Node; //Pointer to the ending node
		targetNode->position.x = end.x;
		targetNode->position.y = end.y;
		Node* current = startNode; //Pointer to the current node being looked at
		
		tempDistance = getDistance(startNode, targetNode); //Finds the distance between the start and end nodes
		current->DistanceFrom = tempDistance;
		current->MoveCost = 1;

		open.push_back(current); //Add the starting node to the open list to be checked
		pathIncomplete = false;

		//Continues until there are either no nodes left to be checked on the grid or the path has been found
		while (open.size() > 0)
		{
			current = open[0]; //Setting current to the next Node in line to be checked
			int toBeErased = 0; //Integer to keep track of which Node to remove from open

			//Finds the Node inside of open that is closest to the target node and sets it to current
			for (int i = 1; i < open.size(); i++)
			{
				if (open[i]->TotalScore() < current->TotalScore() || (open[i]->TotalScore() == current->TotalScore() && open[i]->DistanceFrom < current->DistanceFrom))
				{
					current = open[i];
					toBeErased = i;
				}
			} 

			open.erase(open.begin() + toBeErased); //remove the selected Node from open
			closed.push_back(current); //Place the current node into closed

			//Checks to see if the target node has been reached
			if (current->position.x == targetNode->position.x && current->position.y ==targetNode->position.y)
			{
				return retracePath(startNode, current); //Finds the path from the start to the target
			}

			vector<Node*> neighbor = tempGrid.GetNeighbors(current); //Stores neighbors of the current node to be checked
			for (int g = 0; g < neighbor.size(); g++)
			{
				if (containsNode(neighbor[g], closed)) //Makes sure the node being checked hasnt already been checked
				{

				}
				else
				{
					int newMovementCostToNeighbor = current->MoveCost + getDistance(current, neighbor[g]); //Record distance between node being checked and target node
					//Checks to see if the new node needs to be added to Open
					if (newMovementCostToNeighbor < neighbor[g]->MoveCost || !(containsNode(neighbor[g], open))) 
					{
						//Sets the nodes variables before adding it to Open
						neighbor[g]->setParent(current);
						neighbor[g]->MoveCost = newMovementCostToNeighbor;
						neighbor[g]->DistanceFrom = getDistance(neighbor[g], targetNode);
						if (!containsNode(neighbor[g], open))
						{
							open.push_back(neighbor[g]);
						}
					}
				}

			}
		}
		pathIncomplete = true; //Registers that the correct path was not found
		if (tempPath.size() == 0 || tempPath.size() == 1)
		{
			return tempPath;
		}
		else
		{
			return tempPath;
		}
	}

	int APath::getDistance(Node* start, Node* end)
	{
		return (abs(start->position.x - end->position.x) + (abs(start->position.y - end->position.y)) * 10);
	}

	bool APath::containsNode(Node* check, vector<Node*> target)
	{
		for (int i = 0; i < target.size(); i++)
		{
			if (target[i]->position.x == check->position.x && target[i]->position.y == check->position.y)
			{
				return true;
			}
		}
		return false;
	}

	vector<Node*> APath::retracePath(Node* start, Node* target)
	{
		vector<Node*> path;
		Node* curren = target;
		//Runs until the path from start to target has been fully retraced
		while (curren->position.x != start->position.x && curren->position.x != start->position.y) 
		{
			path.push_back(curren);
			if (!curren->getParent()) //Something has gone wrong and the linked list is broken
			{				
				return tempPath;
			}	
			curren = curren->getParent(); //Moves down the line of nodes by one
		}
		reverse(path.begin(), path.end()); //Reverses the list of Nodes so it starts from the start node and ends at the target
		return path;
	}
