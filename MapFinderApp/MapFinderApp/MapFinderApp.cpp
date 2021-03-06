// MapFinderApp.cpp : Defines the entry point for the console application.
//
#pragma once
#include "stdafx.h"
#include <vector>
#include <stdlib.h>    
#include "MapCreator.h"
#include <iostream>
#include <vector>
#include "APath.h"
#include "Vector2.h"
using namespace std;

int main()
{
	APath pathFinder;
	Grid grid;
	MapCreator map;
	map.CreateLevel();
	map.PrintGrid();
	grid.updateGrid(map.getTaken());

	pathFinder.Start(grid);
	Vector2 start;
	start.x = map.startx;
	start.y = map.starty;
	Vector2 end;
	end.x = map.endx;
	end.y = map.endy;

	vector<Node*> answer;
	answer = pathFinder.findPath(start, end);
	cout << answer.size() << endl;
	for (int i = 0; i < answer.size(); i++)
	{
		cout << answer[i]->position.x << "  " << answer[i]->position.y << endl;
	}

	int x = 0;
		cin >> x;
    return 0;
}

