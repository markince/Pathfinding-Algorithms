// File:	      CMapInput.hpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019

#ifndef _MYFILE
#define _MYFILE

#pragma once

// Class used to read in data from the two text files 
// Contains various functions to read in data and return data back to the main program
class CMapInput
{
private:
	struct mCoords          // Co-ordinates structure
	{
		int x;
		int y;
	};
	string  mMapFile;       // Map File name
	string  mCoordsFile;    // Coords file name
	string  mMapLetter;     // Added to start of filename so complier does not need to be recompiled for different map names
	string  mTempstring;    // Used to concatenate strings together
	fstream mMapInfile;     // Stream used to read in data
	fstream mCoordsInfile;  // Stream used to read in data
	int     mMapXSize;      // X Map Size
	int     mMapYSize;      // Y Map Size
	mCoords mStartXY;       // Start position on map
	mCoords mGoalXY;        // Goal position on map
	vector<vector<int>> mMapFileData; // 2D vector to store map data

public:
	CMapInput           (string mapFile, string coordsFile, string mapLetter); // Constructor
	void OpenMapFile    (); // Opens the map file
	void OpenCoordsFile (); // Opens the Coords file
	void ReadMapFile    (); // Reads in data from the map file
	void ReadCoordsFile (); // Reads in data from the Coords file
	int  GetStartX      (); // Returns start X Position
	int  GetStartY      (); // Returns start Y Position
	int  GetGoalX       (); // Returns goal X Position
	int  GetGoalY       (); // Returns goal Y Position
	int  GetMapXSize    (); // Returns map X Size
	int  GetMapYSize    (); // Returns map Y Size
	int CharToInt       (char c); // Used to convert read in data to characters
	vector<vector<int>> ReturnMapFileDetails();  // Returns the values of the map file to the main program
};

#endif
