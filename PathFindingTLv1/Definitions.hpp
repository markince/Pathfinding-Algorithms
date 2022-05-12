// File:	      Definitions.hpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019
//
// Type definitions to support path finding 
//

#pragma once

#include <vector> // Allows use of vector class functions
#include <deque>  // An indexed sequence container that allows fast insertion and deletion at both its beginning and its end.
#include <memory> // Defines general utilities to manage dynamic memory
using namespace std;

// Constants
const int VALUE_ZERO       = 0; // Constants used for single values
const int VALUE_ONE        = 1;
const int VALUE_TWO        = 2;
const int VALUE_THREE      = 3;
const int VALUE_FOUR       = 4;
const int VALUE_FIVE       = 5;
const int VALUE_TEN        = 10;
const int MOVE_DUMMIES_MAX = 50;

const float SECOND               = 1.0f;  // Value used to work out the fps from the frametime
const float NUM_CUBE_POS         = -1.0f; // Modifies cube position
const float CUBE_Y_POS           = 0.05f; // Cube Y Position on display map
const float CUBE_SCALE           = 0.1f;  // Cube Scale on display map
const float SPACESHIP_Y_POS      = 1.0f;  // Spaceship Y position
const float SPACESHIP_SCALE      = 0.11f; // Spaceship Scale modifier
const float MOVE_DUMMIES_POS_MOD = 0.01f; // Position of dummies modifier
const float CAM_ROTATE_VALUE     = 90.0f; // Used to rotate camera so it is looking downwards
const float CAM_X_POS            = 5.0f;  // Set Camera X position
const float CAM_Y_POS            = 12.0f; // Set Camera Y position
const float CAM_Z_POS            = 4.0f;  // Set Camera Z position
const float SPACESHIP_BOX_SIZE   = 0.01f; // Collision detection box size for spaceship

const string MAP_FILE             = "mMap.txt";       // Various file names
const string COORDS_FILE          = "mCoords.txt";
const string FLOOR_FILE_NAME      = "floorSkin.png";
const string BRICK_FILE_NAME      = "brickTile.jpg";
const string GRASS_FILE_NAME      = "grassTile.jpg";
const string WATER_FILE_NAME      = "waterTile.jpg";
const string cubeNames[VALUE_TEN] = { "zeroTileSkin.png",  "oneTileSkin.png",  "twoTileSkin.png", "threeTileSkin.png",
								      "fourTileSkin.png",  "fiveTileSkin.png", "sixTileSkin.png", "sevenTileSkin.png",
									  "eightTileSkin.png", "nineTileSkin.png" };


struct SNode // Represents a node in the search tree.
{
	int x;             // x coordinate
	int y;             // y coordinate
	int score;         // used in more complex algorithms
	SNode* parent = 0; // Raw pointer used to calculate final path
};

// Lists of nodes (e.g Open, Closed and Paths) are implemented as double-ended queues
using NodeList = deque<unique_ptr<SNode>>;

// Terrain costs for map squares
enum ETerrainCost
{
  Clear = 1,
  Wood  = 2,
  Water = 3,
  Wall  = 0
};

// Maps of any size are implemented as 2D vectors
using TerrainMap = vector<vector<ETerrainCost>>;
