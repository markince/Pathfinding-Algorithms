// File:	      CSearchBestFirst.hpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02-02-2019
//
// Declaration of Search class for Best First algorithm
//

#pragma once

#include "Definitions.hpp"  // Type definitions
#include "Search.hpp"       // Base (=interface) class definition

// Best First search class definition
// Inherit from interface and provide implementation for Best First algorithm
// This class performs a Best-first search. 

class CSearchBestFirst : public ISearch
{
private:
	struct coords // Represents a node in the search tree
	{
		int x;          // x coordinate
		int y;          // y coordinate
		int score;      // used in more complex algorithms
		coords* parent; // Raw pointer used to calculate final path
	};
	deque <unique_ptr<coords>> openList;
	deque <unique_ptr<coords>> closedList;
	deque <unique_ptr<coords>> tmpList;    // Notice the use of a temp list here
	bool foundGoal;                        // Goal has been found
	coords* parentPointer;                  
	enum direction                         // Directions used to calculate neighbouring nodes
	{
		north = 1,
		east = 2,
		south = 3,
		west = 4
	};
public:
	CSearchBestFirst();                                                             // Constructor
	bool FindNodeInList(deque <unique_ptr<coords>>& myList, int xCord, int yCord);  // Search Function
	void DisplayList(deque<unique_ptr<coords>>& myList);                            // Display Function

	// Constructs the path from start to goal for the given terrain
	bool FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path, int xMapSize, int yMapSize);

	~CSearchBestFirst();                                                            // Destructor
};