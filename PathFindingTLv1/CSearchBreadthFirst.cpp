// File:	      CSearchBreadthFirst.cpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02-02-2019
//
// Implementation of Search class for Breadth-first algorithm
//

#include <iostream>
#include "CSearchBreadthFirst.hpp"

// This function takes ownership of the start and goal pointers that are passed in from the calling code.
// Ownership is not returned at the end, so the start and goal nodes are consumed.
// The Path is returned through the reference parameter.

CSearchBreadthFirst::CSearchBreadthFirst() // Constructor included even tho empty
{

}

// This function searches through the open and Closed list to find a certain node.
// The node required and the list to be searched is input into the function when declared
bool CSearchBreadthFirst::FindNodeInList(deque <unique_ptr<coords>>& myList, int xCord, int yCord)
{
	deque <unique_ptr <coords>>::iterator it; 	//Create an iterator

	it = myList.begin(); 	// Point it at the front of the list

	while (it != myList.end()) 	// While not at the end of the list
	{
		// Compare the value of the pointer to the item in the list
		if ((*it)->x == xCord && (*it)->y == yCord)
		{
			return true; // Return true if found
		
		} // End of if statment

		it++; // increment the iterator
	
	} // End of while loop

	return false; // Otherwise return false

} // End of FindNodeInList function


// This function displays the entire list that is passed into it.
void CSearchBreadthFirst::DisplayList(deque<unique_ptr<coords>>& myList)
{
	deque <unique_ptr <coords>>::iterator it; 	//Create an iterator

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
bool CSearchBreadthFirst::FindPath(TerrainMap& terrain, unique_ptr<SNode> start, unique_ptr<SNode> goal, NodeList& path, int xMapSize, int yMapSize)
{

	// Set various starter variables
	foundGoal = false;
	direction nodeDirection; 

	// Create unique temporary pointers used to move data around
	unique_ptr<coords> currentNode(new coords);
	unique_ptr<coords> tempNode(new coords);

	// Push the initial state (start) on to OpenList
	currentNode->x = start->x;
	currentNode->y = start->y;
	currentNode->parent = NULL;
	openList.push_back(move(currentNode));

	while (!foundGoal) 	// Until a goal state is found or OpenList is empty do
	{
		// If OpenList was empty, return failure and quit.
		if (openList.empty())
		{
			cout << "    Search failed, Openlist is empty. " << endl;
			return false;
		}

		// Remove (pop) the first element from OpenList and call it 'current'.
		currentNode = move(openList.front());
		openList.pop_front();

		// If 'current' is the goal state, return success and quit
		if ((currentNode->x == goal->x) && (currentNode->y == goal->y))
		{
			foundGoal = true;
			break;
		}

		// This next section applies the rule to generate a new state
		// If the new state has not already been visited, it adds the new state to end of OpenList.

		nodeDirection = north; // Set it so that it checks the node first

		for (int i = 0; i < VALUE_FOUR; i++) // Check 4 nodes around the current
		{
			switch (nodeDirection)
			{
				case north: // Generate top node
				{
					tempNode->x = currentNode->x;
					tempNode->y = currentNode->y + VALUE_ONE;
					break;
				}
				case east: // Generate node to right of current
				{
					tempNode->x = currentNode->x + VALUE_ONE;
					tempNode->y = currentNode->y;
					break;
				}
				case south: // Generate node below current
				{
					tempNode->x = currentNode->x;
					tempNode->y = currentNode->y - VALUE_ONE;
					break;
				}
				case west: // Generate node to left of current
				{
					tempNode->x = currentNode->x - VALUE_ONE;
					tempNode->y = currentNode->y;
					break;
				}

			} // End of switch statment

			tempNode->parent = currentNode.get(); // Set the parent node (used to calculate path later on)


			// The next statment checks to see if the new state is either on the openList, or the closedList
			// and also if the node is within the boundaries of the map

			if ((!FindNodeInList(openList,   tempNode->x, tempNode->y)) &&
				(!FindNodeInList(closedList, tempNode->x, tempNode->y)) &&
				(tempNode->x >= 0 && tempNode->x < xMapSize) &&
				(tempNode->y >= 0 && tempNode->y < yMapSize))
			{

				if (terrain[tempNode->x][tempNode->y] != 0) // Check if the node is not equal to a wall
				{
					openList.push_back(move(tempNode));     // Push node onto the BACK of the openList
					tempNode.reset(new coords);             // Reset node
				
				} // End of if statment

			} // End of if statment
	
			// Calculate the next node to create a new state for
			if      (nodeDirection == north) nodeDirection = east;
			else if (nodeDirection == east)  nodeDirection = south;
			else if (nodeDirection == south) nodeDirection = west;
			else if (nodeDirection == west)  nodeDirection = north;

		} // End of for loop

		// Display the data of each search loop
		cout << "\n    CURRENT NODE: ";
		cout << "    X: " << currentNode->x << " Y: " << currentNode->y << endl << endl;
		cout << "    Open List:" << endl;
		DisplayList(openList);   // Calls display function to display the lists
		cout << "    Closed List:" << endl;
		DisplayList(closedList);

		closedList.push_back(move(currentNode)); // Adds 'current' node to the ClosedList

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

		deque <unique_ptr <SNode>>::iterator it; //Create an iterator

		it = finalPathList.begin();  // Point it at the front of the list

		while (it != finalPathList.end()) 
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

CSearchBreadthFirst::~CSearchBreadthFirst() // Destructor
{

}