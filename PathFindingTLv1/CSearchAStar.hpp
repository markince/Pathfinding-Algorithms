// File:	      CSearchAStar.hpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02-02-2019
//
// Declaration of Search class for the A* algorithm
//

#pragma once

#include "Definitions.hpp"  // Type definitions
#include "Search.hpp"       // Base (=interface) class definition

// Dijkstra's class definition

// Inherit from interface and provide implementation for A* algorithm
// This class performs an A* search. 

class CSearchAStar : public ISearch
{
private:
	struct coords // Represents a node in the search tree
	{
		int x;          // x coordinate
		int y;          // y coordinate
		int cost;       // used to calculate cost
		int score;      // used to calculate score 
		coords* parent; // Raw pointer used to calculate final path
	};
	deque <unique_ptr<coords>> openList;
	deque <unique_ptr<coords>> closedList;
	bool foundGoal;        // Goal has been found
	coords* parentPointer; // used to calculate final path
	enum direction         // Directions used to calculate neighbouring nodes
	{
		north = 1,
		east  = 2,
		south = 3,
		west  = 4
	};
public:
	CSearchAStar();                                                                 // Constructor
	bool FindNodeInList(deque <unique_ptr<coords>>& myList, int xCord, int yCord);  // Search Function
	void DisplayList(deque<unique_ptr<coords>>& myList);                            // Display Function

	// Constructs the path from start to goal for the given terrain
	bool FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path, int xMapSize, int yMapSize);

	~CSearchAStar();                                                                // Destructor 
};