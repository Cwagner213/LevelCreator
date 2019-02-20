# LevelCreator


Level creator is a C++ application that first generates a layout for a warehouse in a 2d array of chars and then picks two random points within it to find the shortest path between using the A* algorithm.   

----------------------------------------------------------------------------------------------------------------------------------

MapCreator class --- Where all of the map creation happens, all map creation is contained within this class from making the floor layout to setting the walls around it.  Any modifications to the map should be made here.  Uses the 'Room.cpp' and 'Vector2.cpp' classes to put together the map and store information.   ***Needs unit testing added to check maps to make sure everything is reachable***

APath class --- Most of the pathfinding happens here, using the A* algorithm and a 2d array passed in from the main program it will find and return the shortest path between two points. All optimizations to pathfinding should be added here.  
BUG: sometimes it is returning zero.  Either its unable to find a path through or something is going wrong in the algorithm.  Doesn't seem to happen in the game I use it for so not sure what the difference is. Could be made faster using a heap. 

Grid class --- Used mainly to find the neighboring Nodes on a grid for the APath class when called upon. Uses Nodes (similar to a linked list) to represent the grid.   

