// File:	      CSearchBestFirst.cpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02-02-2019
//
// Implementation of Search class for Best First algorithm
//

#include <iostream>
#include <algorithm>
#include "CSearchBestFirst.hpp"

// This function takes ownership of the start and goal pointers that are passed in from the calling code.
// Ownership is not returned at the end, so the start and goal nodes are consumed.
// The Path is returned through the reference parameter.

CSearchBestFirst::CSearchBestFirst() // Constructor included even tho empty
{

}

// This function searches through the open and Closed list to find a certain node.
// The node required and the list to be searched is input into the function when declared
bool CSearchBestFirst::FindNodeInList(deque <unique_ptr<coords>>& myList, int xCord, int yCord)
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

// This function displays the entire list that is passed into it.
void CSearchBestFirst::DisplayList(deque<unique_ptr<coords>>& myList)
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
bool CSearchBestFirst::FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path, int xMapSize, int yMapSize)
{	
	// Set various starter variables
	foundGoal = false;
	direction nodeDirection;

	// Create temporary pointers used to move data around
	unique_ptr<coords> currentNode(new coords);
	unique_ptr<coords> tempNode(new coords);

	// Push the initial state (start) on to OpenList, set its parent to 0 and 
	// calculate its score using the heuristic value
	currentNode->x = start->x;
	currentNode->y = start->y;
	currentNode->score = abs(goal->x - start->x) + abs(goal->y - start->y);
	currentNode->parent = NULL;
	openList.push_back(move(currentNode));

	while (!foundGoal) // Until a goal state is found or OpenList is empty
	{
		currentNode = move(openList.front()); // Remove (pop) the first element from OpenList and call it 'current'.
		openList.pop_front();

		// If 'current' is the goal state, return success and quit
		if ((currentNode->x == goal->x) && (currentNode->y == goal->y))
		{
			foundGoal = true;
			break;
		}

		// This next section applies the rule to generate a new state
		// If the new state has not already been visited, it adds the new state to end of OpenList and calculates
		// its heuristic value

		nodeDirection = north; // Set it so that it checks the node first

		for (int i = 0; i < VALUE_FOUR; i++)
		{
			switch (nodeDirection)
			{
			case north: // Generate top node
			{
				tempNode->x = currentNode->x;
				tempNode->y = currentNode->y + VALUE_ONE;

				// Calcuate heuristic value (Distance from goal)
				tempNode->score = abs(goal->x - currentNode->x) + abs(goal->y - currentNode->y - VALUE_ONE);
				break;
			}
			case east: // Generate node to right of current
			{
				tempNode->x = currentNode->x + VALUE_ONE;
				tempNode->y = currentNode->y;
				tempNode->score = abs(goal->x - currentNode->x - VALUE_ONE) + abs(goal->y - currentNode->y);
				break;
			}
			case south: // Generate node below current
			{
				tempNode->x = currentNode->x;
				tempNode->y = currentNode->y - VALUE_ONE;
				tempNode->score = abs(goal->x - currentNode->x) + abs(goal->y - currentNode->y + VALUE_ONE);
				break;
			}
			case west: // Generate node to left of current
			{
				tempNode->x = currentNode->x - VALUE_ONE;
				tempNode->y = currentNode->y;
				tempNode->score = abs(goal->x - currentNode->x + VALUE_ONE) + abs(goal->y - currentNode->y);
				break;
			}

			} // End of switch statment



			// The next statment checks to see if the new state is either on the openList, or the closedList
			// and also if the node is within the boundaries of the map

			if ((!FindNodeInList(openList, tempNode->x, tempNode->y)) &&
				(!FindNodeInList(closedList, tempNode->x, tempNode->y)) &&
				(tempNode->x >= 0 && tempNode->x < int(terrain.size())) &&
				(tempNode->y >= 0 && tempNode->y < int(terrain.size())))
			{
				if (terrain[tempNode->x][tempNode->y] != 0) // Check if the node is not equal to a wall
				{
					tempNode->parent = currentNode.get(); // Set the parent node (used to calculate path later on)

					openList.push_back(move(tempNode));  // Push node onto the back of the tmpList

					// Sort the openList according to the heuristic values of the elements
					sort(openList.begin(), openList.end(), [](unique_ptr<coords>& lhs,
						unique_ptr<coords>& rhs) { return lhs->score < rhs->score; });

					tempNode.reset(new coords); // Reset node

				} // End of for loop

			} // End of for loop

			// Calculate the next node to create a new state for
			if      (nodeDirection == north) nodeDirection = east;
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

		closedList.push_back(move(currentNode));  // Adds 'current' node to the ClosedList

	} // End of while loop

	// The next section calculates the final path of the map using the parent raw pointer

	if (foundGoal) // Has a goal been found?
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

		return true;

	} 
	else
	{
		return false;
	}

} // End of FindPath function

CSearchBestFirst::~CSearchBestFirst() // Destructor
{

}
