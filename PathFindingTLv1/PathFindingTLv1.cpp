// File:	      PathFindingTLv1.cpp: A pathfinding program written in C++
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019

/*
Program (game) to meet the full specification of CO2301 Games Development 1 Assignment 2019
- An assignment that implements one or more path finding algorithms, with the option of providing a visual representation.
*/

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <iostream>     // Used for Cin/Cout
#include <fstream>      // Input/output stream class to operate on files
#include <sstream>      // Stream class to operate on strings
#include <deque>        // An indexed sequence container that allows fast insertion and deletion at both its beginning and its end. 
#include <memory>       // Defines general utilities to manage dynamic memory
#include <string>       // Includes support for different types of strings
#include <algorithm>    // Defines functions for searching, sorting, counting and manipulating
#include <cstdlib>

#include "Definitions.hpp"          // Definitions header file
#include "Search.hpp"               // Search header file
#include "SearchFactory.hpp"        // Search Factory header file
#include "CSearchBreadthFirst.hpp"  // Breadth-first search class header file
#include "CSearchDepthFirst.hpp"    // Depth-first search class header file
#include "CSearchHillClimbing.hpp"  // Hill-Climbing class header file
#include "CSearchBestFirst.hpp"     // Best-first class header file
#include "CSearchAStar.hpp"         // A* class header file
#include "CMapInput.hpp"            // Map Input class header file

using namespace tle; // TL-Engine Namespace

// Global Functions
void DisplayFinalList (deque<unique_ptr<SNode>>& myList);               // Displays final generated path
void OutputListToFile (deque<unique_ptr<SNode>>& myList);               // Populates output.txt with final path
bool Point2Box        (float pointX,  float pointZ,   float boxXPos,    // Collison detection used for graphical
	                   float boxZPos, float boxDepth, float boxWidth);  // map display

// Key Code Constants used in visual representation

const EKeyCode QUIT_KEY  = Key_Escape;       // Quit the game
const EKeyCode START_KEY = Key_S;            // Start the spaceship moving
const EKeyCode RESET_KEY = Key_W;            // Reset the spaceship


void main()
{

	// Enums
	enum state { START, MOVING, END }; // Used in the graphical representation

	// Varaibles
	vector<vector<int>>   tempETerrain;               // Temporay retrive the data from the MapRead class	
	TerrainMap            mainMap;                    // Enum map used for searches
	NodeList              myList;                     // Final path list
	vector<ETerrainCost>  enumRow;                    // Used to push data onto mainMap
	vector<int>::iterator it;                         // Iterator used to populate enum map
	string                fileLetter;                 // Stores letter selection for choice of map
	string                mapName;                    // Final map filename
	string                coordsName;                 // Final coords filename
	unique_ptr<SNode>     startNode(new SNode);       // Moves start node data into pathFinder class
	unique_ptr<SNode>     goalNode(new SNode);        // Moves goal node data into pathFinder class
	unique_ptr<SNode>     tempNode(new SNode);        // Node used to retreave data from Mylist to move the spaceship
	ISearch*              pPathFinder;                // Pointer to pathfinder class
	char                  quitChoice;                 // User menu choice
	bool                  success;                    // Has a path been found?
	int                   choice;                     // User menu choice
	int                   movePoint = 1;              // Counter used to move the spaceship in the graphical representation
	int                   listSize;                   // Size of list of dummy node positions the spaceship must move to
	float                 spaceShipMoveSpeed = 20.0f;
	float                 frameTime = 0.0f;  // Modifer used through the graphical representation to move objects depending on the frame update time
	float                 fps = 0.0f;  // Frames per second value
	bool                  isMapValid = true;

	// *****************************************************************
	//                        READ IN MAP DETAILS                      *
	// *****************************************************************

	// Set-up Display

	cout << " **************************************************************************\n";
	cout << " *                                                                        *\n";
	cout << " *                         WELCOME TO PATHFINDER!                         *\n";
	cout << " *                    A pathfinding program written by                    *\n";
	cout << " *                              Mark Ince                                 *\n";
	cout << " *                                                                        *\n";
	cout << " **************************************************************************\n";

	cout << "    Please enter the letter of the files you wish to load : ";
	cin >> fileLetter;

	mapName = fileLetter + "Map.txt";
	coordsName = fileLetter + "Coords.txt";

	// Create new instance of class to read in data
	CMapInput* myMaps = new CMapInput(mapName, coordsName, fileLetter);

	myMaps->OpenCoordsFile(); // Open coordinates file
	myMaps->ReadCoordsFile(); // Read in data from coordinates file

	// Set start nodes using data read in from GetMap class
	startNode->x = myMaps->GetStartX();
	startNode->y = myMaps->GetStartY();
	startNode->score = 0;
	startNode->parent = NULL;

	// Set goal nodes using data read in from GetMap class
	goalNode->x = myMaps->GetGoalX();
	goalNode->y = myMaps->GetGoalY();
	goalNode->score = 0;
	goalNode->parent = NULL;

	myMaps->OpenMapFile(); // Open Map file
	myMaps->ReadMapFile(); // Read in data from Map file

	// Retrieve the map data from the readMapFile class and store it in a temp variable
	tempETerrain = myMaps->ReturnMapFileDetails();


	// *****************************************************************
	//                        POPULATE ENUM VECTOR                     *
	// *****************************************************************

	// Populate the mainMap vector that is used to send into the search classes
	for (int i = 0; i < myMaps->GetMapYSize(); i++)
	{
		// Copy each row one by one into a temp 2D Vector
		vector<int> copyRow(tempETerrain[i].begin(), tempETerrain[i].end());

		// Populate the enum vector with correct information
		for (it = copyRow.begin(); it != copyRow.end(); it++)
		{
			if (*it == VALUE_ONE)   enumRow.push_back(Clear);
			else if (*it == VALUE_TWO)   enumRow.push_back(Wood);
			else if (*it == VALUE_THREE) enumRow.push_back(Water);
			else if (*it == VALUE_ZERO)  enumRow.push_back(Wall);

		} // End of for loop

		mainMap.push_back(enumRow); // Pushes the row back onto the 2D vector
		enumRow.clear(); 		    // Clear vector
		copyRow.clear();            // Clear vector

	} // End of for loop

	// Check if start position is a wall
	if (mainMap[myMaps->GetStartX()][myMaps->GetStartY()] == 0)
	{
		cout << "\n    Invalid starting position - the starting position is a wall!\n";
		cout << "    Please check your map input file.\n\n";
		isMapValid = false;
		system("pause");
	}



	// *****************************************************************
	//                         PERFORM SEARCHES                        *
	// *****************************************************************

	if (isMapValid)
	{

		// Display Menu

		cout << endl << endl;

		cout << "    Select the search algorithm you wish to perform: \n\n";
		cout << "    1. Breadth First Search\n";
		cout << "    2. Depth First Search\n";
		cout << "    3. Hill-Climbing\n";
		cout << "    4. Best-First Search\n";
		cout << "    5. A* Algorithm\n\n";
		cout << "    Enter 1-5: ";

		cin >> choice; // Read in user choice

		cout << "\n    Finding path, please wait.....\n";


		switch (choice) // Act on user choice
		{
		case VALUE_ONE:
		{
			pPathFinder = NewSearch(BreadthFirst); // Pointer to search type
			success = pPathFinder->FindPath(mainMap, move(startNode), move(goalNode), myList, myMaps->GetMapXSize(), myMaps->GetMapYSize()); // Perform Search
			break;
		}
		case VALUE_TWO:
		{
			pPathFinder = NewSearch(DepthFirst);
			success = pPathFinder->FindPath(mainMap, move(startNode), move(goalNode), myList, myMaps->GetMapXSize(), myMaps->GetMapYSize());
			break;
		}
		case VALUE_THREE:
		{
			pPathFinder = NewSearch(HillClimbing);
			success = pPathFinder->FindPath(mainMap, move(startNode), move(goalNode), myList, myMaps->GetMapXSize(), myMaps->GetMapYSize());
			break;
		}
		case VALUE_FOUR:
		{
			pPathFinder = NewSearch(BestFirst);
			success = pPathFinder->FindPath(mainMap, move(startNode), move(goalNode), myList, myMaps->GetMapXSize(), myMaps->GetMapYSize());
			break;
		}
		case VALUE_FIVE:
		{
			pPathFinder = NewSearch(AStar);
			success = pPathFinder->FindPath(mainMap, move(startNode), move(goalNode), myList, myMaps->GetMapXSize(), myMaps->GetMapYSize());
			break;
		}

		} // End of switch statment

		// *****************************************************************
		//                   DISPLAY AND OUTPUT FINAL PATH                 *
		// *****************************************************************

		if (success) // Has a path been found?
		{
			cout << "\n    A successful path has been found!\n";
			cout << "\n    Your final path is as follows: \n\n";
			DisplayFinalList(myList); // Display final path list
			OutputListToFile(myList); // output final path list to output.txt
			cout << "\n    Type v to view a visual representation of your path or q to quit : ";
			cout << "\n    v or q : ";
			cin >> quitChoice;
			switch (quitChoice)
			{
			case 'v': break;   // Continue
			case 'q': exit(0); // Break out of program
			}
		}
		else
		{
			cout << "    ERROR : NO PATH FOUND!\n" << endl;
			system("pause");

		} // End of if statment

		// *****************************************************************
		//                    GRAPHICAL REPRESENTATION                     *
		//                      USING THE TL ENGINE                        *
		// *****************************************************************

		if (success)
		{

			// Create a 3D engine (using TLX engine here) and open a window for it
			I3DEngine* myEngine = New3DEngine(kTLX);
			myEngine->StartFullscreen(1920, 1080);

			// Add default folder for meshes and other media
			myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
			myEngine->AddMediaFolder("./media");

			IFont* normalText = myEngine->LoadFont("Arial", 30); // Text for lower left info panel

			// Set-up Meshes
			IMesh* floorMesh = myEngine->LoadMesh("Floor.x");
			IMesh* cubeMesh = myEngine->LoadMesh("Cube.x");
			IMesh* spaceShipMesh = myEngine->LoadMesh("FFstarfighter.x");
			IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");

			// Models
			IModel* floor = floorMesh->CreateModel(0.0f, 0.0f, 0.0f);
			floor->Scale(0.1f);
			IModel* cube;

			// Sprites
			ISprite* infoPanel = myEngine->CreateSprite("keyGuide.png");

			// Create map surface for spaceship to move on
			for (int i = 0; i < myMaps->GetMapXSize(); i++)
			{
				for (int j = 0; j < myMaps->GetMapYSize(); j++)
				{
					cube = cubeMesh->CreateModel(0.0f + static_cast<float>(i), 0.0f, 0.0f + static_cast<float>(j));
					cube->Scale(CUBE_SCALE);
					cube->SetSkin(FLOOR_FILE_NAME);

				} // End of for loop

			} // End of for loop

			// Number Cubes
			IModel* cubeXNums[VALUE_TEN];
			IModel* cubeZNums[VALUE_TEN];

			for (int i = 0; i < VALUE_TEN; i++)
			{
				cubeXNums[i] = cubeMesh->CreateModel(0.0f + i, 0.0f, NUM_CUBE_POS);
				cubeXNums[i]->Scale(CUBE_SCALE);
				cubeXNums[i]->SetSkin(cubeNames[i]);

				cubeZNums[i] = cubeMesh->CreateModel(NUM_CUBE_POS, 0.0f, 0.0f + i);
				cubeZNums[i]->Scale(CUBE_SCALE);
				cubeZNums[i]->SetSkin(cubeNames[i]);
			}

			// This next section sets up the graphical map. It uses the data from the mainMap array and reskins the cubes 
			// depending on the surface value. There are four types of surface. Normal, Wall, grass and water
			for (int i = 0; i < VALUE_TEN; i++)
			{
				for (int j = 0; j < VALUE_TEN; j++)
				{
					if (mainMap[i][j] == VALUE_ZERO) // Wall cubes
					{
						IModel* wallCube = cubeMesh->CreateModel(static_cast<float>(i), CUBE_Y_POS, static_cast<float>(j));
						wallCube->Scale(CUBE_SCALE);
						wallCube->SetSkin(BRICK_FILE_NAME);
					}
					else if (mainMap[i][j] == VALUE_TWO) // Grass cubes
					{
						IModel* wallCube = cubeMesh->CreateModel(static_cast<float>(i), CUBE_Y_POS, static_cast<float>(j));
						wallCube->Scale(CUBE_SCALE);
						wallCube->SetSkin(GRASS_FILE_NAME);
					}
					else if (mainMap[i][j] == VALUE_THREE) // Water Cubes
					{
						IModel* wallCube = cubeMesh->CreateModel(static_cast<float>(i), CUBE_Y_POS, static_cast<float>(j));
						wallCube->Scale(CUBE_SCALE);
						wallCube->SetSkin(WATER_FILE_NAME);
					}

				}
			} // End of for loop

			// Set up SpaceShip
			tempNode = move(myList.front()); // Get the spaceship start position
			myList.pop_front();

			IModel* spaceShip = spaceShipMesh->CreateModel();
			spaceShip->SetPosition(static_cast<float>(tempNode->x), SPACESHIP_Y_POS, static_cast<float>(tempNode->y));
			spaceShip->Scale(SPACESHIP_SCALE);
			tempNode.reset(new SNode);


			// Movement Dummys
			IModel* movementDummies[MOVE_DUMMIES_MAX];


			listSize = myList.size(); // Get the size of the final path list


			for (int i = 1; i <= listSize; i++)
			{
				tempNode = move(myList.front()); // Get X and Y values from the list
				myList.pop_front();

				// Place the movement dummies around the map
				movementDummies[i] = dummyMesh->CreateModel(static_cast<float>(tempNode->x) + MOVE_DUMMIES_POS_MOD,
					SPACESHIP_Y_POS, static_cast<float>(tempNode->y));
				tempNode.reset(new SNode);

			} // End of for loop


			// Manual camera at location and angle specified
			ICamera* myCamera = myEngine->CreateCamera(kFPS);
			myCamera->RotateX(CAM_ROTATE_VALUE);
			myCamera->SetX(CAM_X_POS);
			myCamera->SetY(CAM_Y_POS);
			myCamera->SetZ(CAM_Z_POS);

			// Set start state of spaceship
			state spaceShipState = START;

			// Call the timer just before the game loop
			myEngine->Timer();

			// The main game loop, repeat until engine is stopped
			while (myEngine->IsRunning())
			{
				// Draw the scene
				myEngine->DrawScene();

				// Set-up the timer:
				// -----------------

				// Call the Timer again and store it in a variable
				frameTime = myEngine->Timer();

				// Calculate the Frames per second
				// Calculated by deviding the frametime by 1
				fps = SECOND / frameTime;

				// Info panel
				infoPanel->SetPosition(1500, 800);

				switch (spaceShipState)
				{
				case START:
				{
					if (myEngine->KeyHit(START_KEY))
					{
						spaceShipState = MOVING;

					} // End of if statment

					break;
				}
				case MOVING:
				{
					if (movePoint <= listSize)
					{
						spaceShip->LookAt(movementDummies[movePoint]);
						spaceShip->Scale(SPACESHIP_SCALE);
						spaceShip->MoveLocalZ(spaceShipMoveSpeed * frameTime);
						if (Point2Box(spaceShip->GetX(), spaceShip->GetZ(), movementDummies[movePoint]->GetX(),
							movementDummies[movePoint]->GetZ(), SPACESHIP_BOX_SIZE, SPACESHIP_BOX_SIZE))
						{
							movePoint++;

						} // End of if statment
					}
					else
					{
						spaceShipState = END;

					} // End of if statment

					break;
				}
				case END:
				{
					break;
				}

				} // End of switch statment

				if (myEngine->KeyHit(RESET_KEY))
				{
					spaceShip->SetPosition(static_cast<float>(tempNode->x), SPACESHIP_Y_POS, static_cast<float>(tempNode->y));
					movePoint = VALUE_ONE;

				} // End of if statment


				// Quit game
				if (myEngine->KeyHit(QUIT_KEY))
				{
					myEngine->Stop(); // stop the engine (will exit the game loop)
				}
			}

			// Delete pointers
			delete (pPathFinder);

			// Delete the 3D engine now we are finished with it
			myEngine->Delete();
		}
	}
} // End of isMapValid if statment

// Functions

// This function is used to display the final path list to the screen. It uses an iterator to move through 
// the list to display the results
void DisplayFinalList(deque<unique_ptr<SNode>>& myList)
{
	int mCount = VALUE_ZERO;  // Counter
	deque <unique_ptr <SNode>>::iterator it; // Iterator to move through the list

	it = myList.begin(); 	// Point at the front of the list

	while (it != myList.end()) 	// While not at the end of the list
	{
		// Display the item the iterator is currently pointing at
		cout << "    X - " << (*it)->x << " Y - " << (*it)->y;
		if (mCount == VALUE_ZERO)                cout << "  (Start Node)";
		if (mCount == myList.size() - VALUE_ONE) cout << "  (Goal)";
		cout << endl;
		it++; 		// Increment the iterator
		mCount++;	// Increment the Counter
	}
} // End of DisplayFinalList function

// This function is used to output the final path list to the "output.txt" file. 
// It uses an iterator to move through the list to display the results.
void OutputListToFile(deque<unique_ptr<SNode>>& myList)
{
	ofstream outfile("output.txt"); // Open file for output
	if (!outfile) // Not sure if this is needed as if there is no file, the complier should make one for you
	{
		cout << "Unable to output path to output.txt" << endl;
	}
	else
	{
		deque <unique_ptr <SNode>>::iterator it; // Iterator to move through the list

		it = myList.begin(); // Point at the front of the list

		// While not at the end of the list
		while (it != myList.end())
		{
			// Copy details to output file
			outfile << (*it)->x << " " << (*it)->y << endl;
			it++; // Increment the Counter
		}
		// Display output message
		cout << "\n    The final path route has been written to the file output.txt\n\n";
	}
} // End of OutputListToFile function

// Function to detect collision detection in the TL-Engine between a single point and a box
bool Point2Box(float pointX, float pointZ, float boxXPos, float boxZPos, float boxDepth, float boxWidth)
{

	// Calculate bounding area around the box used for collision detection
	float minX = boxXPos - boxWidth / VALUE_TWO;
	float maxX = boxXPos + boxWidth / VALUE_TWO;
	float minZ = boxZPos - boxDepth / VALUE_TWO;
	float maxZ = boxZPos + boxDepth / VALUE_TWO;

	// Return true if the point is within the bounding box
	// else return false
	return(pointX > minX && pointX < maxX && pointZ > minZ && pointZ < maxZ);

} // End of Point2Box function
