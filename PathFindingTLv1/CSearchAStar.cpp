// File:	      CSearchAStar.cpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02-02-2019
//
// Implementation of Search class for A* algorithm
//

#include <iostream>
#include <algorithm>
#include "CSearchAStar.hpp"

// This function takes ownership of the start and goal pointers that are passed in from the calling code.
// Ownership is not returned at the end, so the start and goal nodes are consumed.
// The Path is returned through the reference parameter.

CSearchAStar::CSearchAStar() // Constructor included even tho empty
{

}

// This function searches through the open and Closed list to find a certain node.
// The node required and the list to be searched is input into the function when declared
bool CSearchAStar::FindNodeInList(deque <unique_ptr<coords>>& myList, int xCord, int yCord)
{
	deque <unique_ptr <coords>>::iterator it; 	// Create an iterator

	it = myList.begin(); 	// Point it at the front of the list

	while (it != myList.end()) 	// While not at the end of the list
	{
		// Compare the value of the pointer to the item in the list
		if ((*it)->x == xCord && (*it)->y == yCord)
		{
			return true; // Return true if found

		} // End of if statment

		it++; // increment the iterator
	}
	return false; // Otherwise return false

} // End of FindNodeInList function

void CSearchAStar::DisplayList(deque<unique_ptr<coords>>& myList)
{
	deque <unique_ptr <coords>>::iterator it; // Create an iterator

	it = myList.begin(); // Point it at the front of the list

	while (it != myList.end()) 	// While not at the end of the list
	{
		// Display the item the iterator is currently pointing at
		cout << "    X : " << (*it)->x << " Y : " << (*it)->y << endl;

		// Increment the iterator
		it++;

	} // End of while loop

	cout << endl;

} // End of DisplayList function

// The next function performs the actual search. It takes in the terrain map generated in the main program and
// the start node and goal nodes and populates the path. The path is list is called by reference so it is populated with nodes
//  so it can be used in the main function
bool CSearchAStar::FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path, int xMapSize, int yMapSize)
{
	// Set various starter variables
	foundGoal          = false;
	int newCost        = 0;
	int heuristicValue = 0;    // Used to calculate the huristic (manhattan distance)
	direction nodeDirection;
	int numOfSorts = 0;

	// Create temporary pointers used to move data around
	unique_ptr<coords> currentNode(new coords);
	unique_ptr<coords> n(new coords);

	// Push the initial state (start) on to OpenList, set its parent to 0 and 
	// calculate its score using the heuristic value
	currentNode->x = start->x;
	currentNode->y = start->y;
	currentNode->score = 0;
	currentNode->cost = 0;
	currentNode->parent = NULL;
	openList.push_back(move(currentNode));

	while (!foundGoal) // Until a goal state is found or OpenList is empty
	{
		// Pop the first (best) element from OpenList and call it 'current'.
		currentNode = move(openList.front());
		openList.pop_front();

		// If 'current' is the goal state, return success, construct the path and quit.
		if ((currentNode->x == goal->x) && (currentNode->y == goal->y))
		{
			foundGoal = true;
			break;
		}

		// This next section applies the rule to generate a new state
		// Applies the rule to generate a new state and call it 'n'

		nodeDirection = north;

		for (int i = 0; i < VALUE_FOUR; i++)
		{
			switch (nodeDirection)
			{
				case north: // Generate top node
				{
					n->x = currentNode->x;
					n->y = currentNode->y + VALUE_ONE;
					break;
				}
				case east:  // Generate node to right of current
				{
					n->x = currentNode->x + VALUE_ONE;
					n->y = currentNode->y;
					break;
				}
				case south: // Generate node below current
				{
					n->x = currentNode->x;
					n->y = currentNode->y - VALUE_ONE;
					break;
				}
				case west: // Generate node to left of current
				{
					n->x = currentNode->x - VALUE_ONE;
					n->y = currentNode->y;
					break;
				}
			} // End of switch statment

			if ((n->x >= 0 && n->x < int(terrain.size())) && // Check if new node is within range of the map
				(n->y >= 0 && n->y < int(terrain.size())) &&
				(terrain[n->x][n->y] != 0))				// Check if the new node is not a wall
			{
				// Calculate cost
				// The cost of the new state is the cost of 'current' added to the cost of moving from 'current' to 'n':
				// newcost = current.cost + cost(current, n)
				if      (nodeDirection == north) newCost = currentNode->cost + terrain[currentNode->x][currentNode->y + 1];
				else if (nodeDirection == east)  newCost = currentNode->cost + terrain[currentNode->x + 1][currentNode->y];
				else if (nodeDirection == south) newCost = currentNode->cost + terrain[currentNode->x][currentNode->y - 1];
				else if (nodeDirection == west ) newCost = currentNode->cost + terrain[currentNode->x - 1][currentNode->y];

				// Next it checks if 'n' is on OpenList or on ClosedList and if the newcost >= existing_n.cost 
				// If so then it will skip the rest of the loop and go back to generating a new node
				if (((FindNodeInList(openList, n->x, n->y)) || 
					(FindNodeInList(closedList, n->x, n->y))) && 
					(newCost >= currentNode->cost))
				{
					goto direction; // Used a goto command here to skip the next statments as "break" did not work successfully
				
				} // End of if statment

				// Check to see if 'n' is on OpenList or on ClosedList 
				// If so then let 'n' now refer to existing_n
				if ((FindNodeInList(openList, n->x, n->y)) || (FindNodeInList(closedList, n->x, n->y)))
				{
					n->x = currentNode->x;
					n->y = currentNode->y;

				} // End of if statment

				// Set the parent of 'n' to 'current'.   
				n->parent = currentNode.get();

				// Calculate the heuristic value.
				heuristicValue = abs(goal->x - n->x) + abs(goal->y - n->y);

				// Set the score of 'n' to newcost + heuristic value.
				n->score = newCost + heuristicValue;

				// Set the cost of 'n' to newcost.
				n->cost = newCost;

				// If 'n' was on ClosedList then remove it from ClosedList and push it onto OpenList.
				if (FindNodeInList(closedList, n->x, n->y))
				{
					// Remove from closed list
					auto p = closedList.begin();
					while (p != closedList.end())
					{
						if ((*p)->x == n->x && (*p)->y == n->y)
						{
							closedList.erase(p);
							break;

						} // End of if statment

						p++; // advance through the loop

					} // End of while loop

					// Push onto OpenList
					openList.push_back(move(n));

				} // End of if statment
				else
				// If 'n' was not on OpenList or on ClosedList then push 'n' on to OpenList.
				if ((!FindNodeInList(openList, n->x, n->y)) &&
					(!FindNodeInList(closedList, n->x, n->y)))
				{
					openList.push_back(move(n));

				} // End of if statment

				// Next we have to Reorder OpenList by score 
				// (since either the cost of 'n' has been updated or 'n' has been pushed onto OpenList).
				sort(openList.begin(), openList.end(), [](unique_ptr<coords>& lhs,
					unique_ptr<coords>& rhs) { return lhs->score < rhs->score; });
				numOfSorts++;

				// Reset node and varibale values
				n.reset(new coords);
				newCost        = 0;
				heuristicValue = 0;

			} // End of if statment

			// Calculate the next node to create a new state for
			// notice the use of "direction" here, (where the program jumps to from "goto")
			direction:
			if     (nodeDirection == north)  nodeDirection = east;
			else if (nodeDirection == east)  nodeDirection = south;
			else if (nodeDirection == south) nodeDirection = west;
			else if (nodeDirection == west)  nodeDirection = north;

		} // End of foor loop

		// Display the data of each search loop
		cout << "\n    CURRENT NODE: ";
		cout << "    X: " << currentNode->x << " Y: " << currentNode->y << endl << endl;
		cout << "    Open List:" << endl;
		DisplayList(openList); // Calls display function to display the lists
		cout << "    Closed List:" << endl;
		DisplayList(closedList);
		

		closedList.push_back(move(currentNode)); // Adds 'current' node to the ClosedList



	} // End of while loop

	// The next section calculates the final path of the map using the parent raw pointer

	if (foundGoal)  // Has a goal been found?
	{
		// Create path and pointer for producing final path list
		deque<unique_ptr<SNode>> finalPathList;
		unique_ptr<SNode> pathNode(new SNode);

		// Push goal node onto final path list
		pathNode->x = goal->x;
		pathNode->y = goal->y;
		pathNode->score = 0;
		pathNode->parent = NULL;
		path.push_back(move(pathNode));
		pathNode.reset(new SNode);

		// Work out the final path using the parent pointer
		parentPointer = currentNode->parent;
		while (parentPointer)
		{
			pathNode->x = parentPointer->x;
			pathNode->y = parentPointer->y;
			finalPathList.push_back(move(pathNode));
			pathNode.reset(new SNode);
			parentPointer = parentPointer->parent; // get the previous parent raw pointer 

		} // End of while loop

		// This next section reverses the finalPathList so that is will be returned the correct way round

		deque <unique_ptr <SNode>>::iterator it; // Create an iterator

		it = finalPathList.begin(); // Point it at the front of the list

		while (it != finalPathList.end()) // While not at the end of the list
		{
			path.push_front(move(*it)); // Push the value to the font of the list

			it++; // Increment the iterator

		} // End of while loop

		cout << "     The openlist has been sorted: " << numOfSorts << " times." << endl;

		return true;

	} 
	else
	{
		return false;
	}

} // End of FindPath function

CSearchAStar::~CSearchAStar()
{

}