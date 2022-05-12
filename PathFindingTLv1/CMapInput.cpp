// File:	      CMapInput.cpp
// Author:	      Mark Ince
// Date:	      28-12-2018
// Last Edited:   02/02/2019


#include <iostream> // Used for Cin/Cout
#include <fstream>  // Input/output stream class to operate on files
#include <sstream>  // Stream class to operate on strings
#include <string>   // Includes support for different types of strings
#include <vector>   // Allows use of vector class functions
using namespace std;

#include "CMapInput.hpp"
#include "Definitions.hpp"

// Fuction to rotate the map the correct way
template<typename T>
void rotateArray(vector<vector<T>> &v)
{
	vector<vector<T>>rotated;
	for (size_t i = 0; i < v[0].size(); i++) {
		vector<T> newRow;
		for (int j = v.size() - 1; j >= 0; j--) {
			newRow.push_back(v[j][i]);
		}
		rotated.push_back(newRow);
	}
	v = rotated;
}


// Constructor. Reads in names of mapFile and Coords file and the first letter.
CMapInput::CMapInput(string mapFile, string coordsFile, string mapLetter)
{
	mMapFile    = mapFile;      
	mCoordsFile = coordsFile;
	mMapLetter  = mapLetter;
}

// Open the mapFile
void CMapInput::OpenMapFile()
{
	cout << "    Looking for " << mMapLetter << "Map.txt Input file......" << endl;
	mMapInfile.open(mMapFile);
	if (!mMapInfile) // If unable to open open
	{
		cout << "    ERROR: Unable to open Map Input file! \n\n";
		system("pause");
		exit(0); // Quit out of the program
	}
	else
	{
		cout << "    Map Input File Loaded Successfully! \n";
	} // End of if statment

} // End of OpenMapFile Function

// Open the Coords File
void CMapInput::OpenCoordsFile()
{
	cout << "\n    Looking for " << mMapLetter << "Coords.txt Input file......" << endl;
	mCoordsInfile.open(mCoordsFile);

	if (!mCoordsInfile) // If unable to open open
	{
		cout << "    ERROR: Unable to open Coords Input file! \n";
	}
	else
	{
		cout << "    Coords Input File Loaded Successfully! \n";
	} // End of if statment

}// End of OpenCoordsFile Function




// This function reads in the data from the Map text file and places it in the correct variables
void CMapInput::ReadMapFile()
{
	//Temp variables used to read in data
	char        mCh;        
	int         mTempNum;
	vector<int> mTempVector;
	const int   NUM_ROTATIONS = 3;
	
	if (mMapInfile.is_open())  // If successfully opened
	{
		// Get First Line (map size)
		getline(mMapInfile, mTempstring);
		istringstream is(mTempstring);
		is >> mMapXSize >> mMapYSize;

		// Read in data grid
		// Reads data in character by character, converts them to an integer and then places them into a vector
		// which is then pushed into a 2D vector line by line.
		for (int row = 0; row < mMapYSize; row++)
		{
			for (int column = 0; column < mMapXSize; column++)
			{
				mMapInfile >> mCh;                // Read in character
				mTempNum = CharToInt(mCh);        // Convert to integer
				mTempVector.push_back(mTempNum);  // Push onto temp vector
			}
			mMapFileData.push_back(mTempVector);  // Place each line in 2D vector
			mTempVector.clear();                  // Clear vector

		} // End of for loop

	} // End of if statment


	// This next two lines rotatate the map around the correct way. Still unsure why this is needed
	// It uses a rotate function (at top of file)
	// If the map is square it rotates it once, if it is rectangular it rotates it 3 times
	// Currently working on trying to read it in correctly so this is not needed

	
	if (mMapXSize == mMapYSize) rotateArray(mMapFileData);
	else for (int i = 0; i < 3; i++) rotateArray(mMapFileData);


} // End of ReadMapFile function

// This function reads in the data from the Coords text file and places it in the correct variables

void CMapInput::ReadCoordsFile()
{
	if (mCoordsInfile.is_open()) // is the file open?
	{
		// Get First Line
		getline(mCoordsInfile, mTempstring);
		istringstream is(mTempstring);
		is >> mStartXY.x >> mStartXY.y; // Set start X and Y positons

		// Get line 2
		getline(mCoordsInfile, mTempstring);
		istringstream is2(mTempstring);
		is2 >> mGoalXY.x >> mGoalXY.y; // Set goal X and Y positons

	} // End of if statment

} // End of for loop

// Function to return Start X Position
int CMapInput::GetStartX()
{
	return mStartXY.x;

} // End of GetStartX function

// Function to return Start Y Position
int CMapInput::GetStartY()
{
	return mStartXY.y;

} // End of GetStartY function

// Function to return goal X Position
int CMapInput::GetGoalX()
{
	return mGoalXY.x;

} // End of GetGoalX function

// Function to return goal Y Position
int CMapInput::GetGoalY()
{
	return mGoalXY.y;

} // End of GetGoalY function

// Function to return X Map Size
int CMapInput::GetMapXSize()
{
	return mMapXSize;

} // End of GetMapXSize function

// Function to return Y Map Size
int CMapInput::GetMapYSize()
{
	return mMapYSize;

} // End of GetMapYSize function

// Function to return the 2D vector that stores the map details back to the main program
vector<vector<int>> CMapInput::ReturnMapFileDetails()
{
	return mMapFileData;

} // End of ReturnMapFileDetails function

// This next function is used within the ReadMapFile function to convert the characters read in
// from the text file into integers
int CMapInput::CharToInt(char c)
{
	int arr[] = { 0,1,2,3,4,5,6,7,8,9 };
	return arr[c - '0'];

} // End of CharToInt function




